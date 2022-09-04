## Requirements

- compiler supporting **C++11**
- **Python3** with the **numpy**, **matplotlib** and **pandas** libraries in order to execute the drawing script `plot.py`

## Compilation

Execute the Bash script

```bash
./compile.sh
```

or manually execute the commands

```bash
mkdir -p build  # create directory if it does not exist
cd build        # move to the directory build
cmake ..        # execute cmake
make            # compile the project
cd ..           # move to the root of the project
```

## Execution

### Execute predefined experimentations

From the root of the project, the prototype can be executed using

```bash
./build/run
```

It will display the available experimentations.

To run the experimentation labeled by the integer `n`, execute

```bash
./build/run n
```

To run all the experimentations, execute

```bash
./build/run 0
```

*WARNING*: some experimentations are very slow and this command can take multiple minutes to terminate.

The results are written in the directory `outputs`. For each experimentation, two files are written:
- `name.info` which summarizes the parameters and the time of the computation
- `name.dat` which stores boxed over-approximations for each time-step

### Modify computation parameters of predefined experimentations

The parameters for the computation are defined in the file `experimentations.cpp`.
Each experimentation is defined in its own function settings:
- `dt` the time-step for the integration
- `tEnd` the final time of the computation
- `order` the order of the Taylor models (maximal order of the symbolic multivariate polynomials)

### Defining its own experimentations

The models are defined as C++11 classes.

1. create a class inheriting the public class `ODE`:
- `SubFunctions getHi() const` defines the vector-valued function `h`
- `SubInputFunctions getGij() const` defines the matrix-valued function `g`
- `StateDomain getStateDomain() const` defines the set of possible initial states
- `InputsDomains getInputsDomains() const` defines the range of the uncertainties `u`
- `int getDim() const` defines the number of state variables
- `std::vector<std::string> getNames() const` defines the names of the state variables
- `std::string getNameODE() const` defines the name of the experimentation

You can use the files `NonLinearODE.hpp` and `NonLinearODE.cpp` as template.

2. create a dedicated function in the files `experiementations.hpp` and `experimentations.cpp`:
   1. use the function `void experimentationNonLinear()` as template
   2. replace the printed message `std::string msg`
   3. replace the pointer to the object encoding the experimentation `ODE* ode`
   4. set the parameters for the computation `double dt`, `double tEnd` and `int order`

3. in the file `main.cpp`, add your experimentation to the vector `experiments` and add a new line to the message "Usage" with an incremented index with respect to the previous one.

Now, the experimentation should be executable from the command line using the last index as argument.

## Draw results

After the execution of some experimentations, we can draw some projections of the computed over-approximation of the reachable set. A script `plot.py` is provided to draw multiple over-approximations of an experimentation `experimentation` with respect to the same dimension:

```bash
./plot.py outputs/experimentation.dat x y1 y2 ...
```

or

```bash
python3 plot.py outputs/experimentation.dat x y1 y2 ...
```

with `x` the ID of the state variable used as abscissa and `y1`, `y2`, etc. the ID of which we want to draw the over-approximations.
The IDs start from 1 and ID 0 corresponds to the time variable.

All the over-approximations with respect to the time are drawn if no IDs are specified:

```bash
./plot.py outputs/experimentation.dat
```
