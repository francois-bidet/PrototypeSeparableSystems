#include "Solver.hpp"

#include "AffineDecomposer.hpp"
#include "NullPrinter.hpp"

const int DEBUG = 1;

Solver::Solver(double dt)
  : ode(nullptr)
{
  this->enclosureComputation = new EnclosureComputation();
  this->decomposer = new AffineDecomposer();
  this->polyExpansion = new PolynomialExpansion();
  this->remainderComputation = new RemainderComputation();
  this->lifter = new Lifter();
  this->printer = new NullPrinter();

  this->contractorTM = new Contractor();

  this->bIntegrator = new BasicIntegrator();
  this->dIntegrator = new DecomposedIntegrator(NB_INTERMEDIATE_MONOMIALS_MAX);

  setDt(dt);
}

Solver::~Solver() {
  delete enclosureComputation;
  delete decomposer;
  delete polyExpansion;
  delete remainderComputation;
  delete lifter;
  delete bIntegrator;
  delete dIntegrator;
  delete printer;
  delete contractorTM;
}

void Solver::setEnclosureComputation(EnclosureComputation* ec) {
  delete this->enclosureComputation;
  this->enclosureComputation = ec;
}

void Solver::setPrinter(Printer* p) {
  delete this->printer;
  this->printer = p;
  setNamesForPrinter();
}

void Solver::setODE(ODE* ode) {
  this->ode = ode;

  int dim = ode->getDim();

  this->s0 = lifter->liftState(ode->getStateDomain());
  this->u = lifter->liftInputs(ode->getGij(), ode->getInputsDomains(), dim+1);

  std::vector<std::string> varNames = ode->getNames();

  this->names.resize(dim+1);
  for(int d = 0; d < dim; ++d) {
    this->names[d+1] = varNames[d];
  }
  int n = 1;
  int dimI = ode->getGij().size();
  int dimJ = (dimI > 0) ? ode->getGij()[0].size() : 0;
  for(int i = 0; i < dimI; ++i)
    for(int j = 0; j < dimJ; ++j) {
      for(int k = 0; k < 2; ++k)
        this->names.push_back("u_{" + std::to_string(n+k) + "}");
      n += 2;
    }
  // for(int d = 0; d < dim; ++d)
  //   this->names.push_back("e_{" + std::to_string(d) + "}");
  this->names[0] = "dt";

  this->bIntegrator->setODE(ode);
  this->dIntegrator->setInputs(this->u);

  setNamesForPrinter();
}

void Solver::setDt(double dt) {
  this->dt = dt;
  SetsLibrary::Interval dtInt(0,dt);
  this->dtTM = this->lifter->lift(dtInt, 0);
  this->bIntegrator->setDtDomain(dtInt);
  this->dIntegrator->setDtDomain(dtInt);
}

void Solver::setTEnd(double t) {
  this->tEnd = t;
}

void Solver::setOrder(int order) {
  this->order = order;
  TaylorModel::setOrder(order);
}

double Solver::getDt() const {
  return this->dt;
}

void Solver::init(ODE* ode,
                  double tEnd,
                  int order,
                  double t0) {
  this->contractorTM->setIntegrator(this->dIntegrator);
  this->polyExpansion->setIntegrator(this->dIntegrator);
  this->enclosureComputation->setIntegrator(this->bIntegrator);
  this->remainderComputation->setIntegrator(this->dIntegrator);
  this->remainderComputation->setContractor(this->contractorTM);

  setODE(ode);
  setTEnd(tEnd);
  setOrder(order);

  setT0(t0);
}

void Solver::setT0(double t0) {
  this->t0 = t0;
}

State Solver::step() {
  Printer* printerLog;
  if(DEBUG >= 20) {
    printerLog = new Printer(std::clog);
    printerLog->setNames(this->names);
    std::clog << "s0:" << std::endl;
    printerLog->print(s0);
  }

  if(DEBUG >= 10)
    std::clog << "RawEnclosure...\n" << std::flush;
  computeRawEnclosure();
  if(DEBUG >= 20)
    printerLog->print(rawEnclosure);

  if(DEBUG >= 10)
    std::clog << "Decompositions...\n" << std::flush;
  computeDecompositions();
  if(DEBUG >= 100) {
    int dim = s0.size();
    State sTmp(dim);
    for(int d = 0; d < dim; ++d)
      sTmp[d] = TaylorModel(MultiPolynomial::createIdentity(d+1));
    Time tTmp(MultiPolynomial::createIdentity(0));

    std::clog << "Decompositions:" << std::endl;
    int nb = this->decompositions.size();
    for(int i = 0; i < nb; ++i) {
      this->decompositions[i].eval(tTmp,sTmp);
      std::clog << "pos[" << i << "]:\n";
      std::vector<TaylorModel> pos(1,this->decompositions[i].getPos());
      printerLog->print(pos);
      std::clog << "neg[" << i << "]:\n";
      std::vector<TaylorModel> neg(1,this->decompositions[i].getNeg());
      printerLog->print(neg);
    }
  }

  if(DEBUG >= 10)
    std::clog << "PolynomialExpansion...\n" << std::flush;
  computePolynomialExpansion();
  if(DEBUG >= 20)
    printerLog->print(approximation);

  if(DEBUG >= 10)
    std::clog << "ValidRemainder...\n" << std::flush;
  computeValidRemainder();
  if(DEBUG >= 20)
    printerLog->print(approximation);

  if(DEBUG >= 20) {
    delete printerLog;
  }

  return approximation;
}

OverApproximations Solver::compute() {
  this->printer->printNames();

  int dim = s0.size();
  int nbInputs = 2 * u.size() * u[0].size();
  int nbTmpVars = dim;
  std::vector<int> idsInputs(nbInputs);
  for(int i = 0; i < nbInputs; ++i)
    idsInputs[i] = i + dim + 1;

  int idFresh = dim+nbInputs+1;
  OverApproximations result;
  while(t0 < tEnd) {
    if(DEBUG >= 1)
      std::clog << "time: " << t0 << "/" << tEnd << std::endl;

    step();

    OverApproximation overapprox;
    overapprox.t0 = this->t0;
    overapprox.dt = std::min(this->dt,tEnd - t0);
    overapprox.state = this->approximation;

    result.push_back(overapprox);
    this->printer->print(overapprox);
    this->printer->flush();

    // update
    this->t0 += dt;
    for(int d = 0; d < ode->getDim(); ++d) {
      TaylorModel tm = this->approximation[d].partialEval(0, 1); // last point of time
      this->s0[d] = tm.partialUnitEval(idsInputs);
      //this->s0[d] = this->s0[d].truncate(BETWEEN_STEPS_ORDER);
      this->s0[d] = this->s0[d].reduce(NB_MONOMIALS_MAX);
      SetsLibrary::Interval r = this->s0[d].getRemainder();
      this->s0[d].setRemainder(SetsLibrary::Interval(0));
      TaylorModel tmpTM = this->lifter->lift(r, idFresh++);
      this->s0[d] += tmpTM;
    }
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

void Solver::computeRawEnclosure() {
  int dim = s0.size();

  this->rawEnclosure = enclosureComputation->compute(s0, dtTM);
}

void Solver::computeDecompositions() {
  SubFunctions hi = ode->getHi();
  int m = hi.size();

  Time dtInt(MultiPolynomial(), SetsLibrary::Interval(0,dt));

  FunctionsImages imgs(m);
  for(int i = 0; i < m; ++i)
    imgs[i] = hi[i](dtInt, rawEnclosure).eval();

  this->decompositions = decomposer->decompose(hi, imgs);

  this->dIntegrator->setDecompositions(this->decompositions);
}

void Solver::computePolynomialExpansion() {
  this->approximation = polyExpansion->compute(s0, dtTM, order);
}

void Solver::computeValidRemainder() {
  this->approximation = remainderComputation->compute(s0, dtTM, approximation);
}

void Solver::setNamesForPrinter() {
  int dim = this->s0.size();
  std::vector<std::string> printNames = this->names;
  if(printNames.size() > 0) {
    printNames[0] = "t";
    printNames.resize(dim+1); // keep only state's variables and time
  }
  this->printer->setNames(printNames);
}
