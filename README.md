# ASAP - a cpp build tool
as simple as possible to use.

## philosophy
1. be as simple as possible to use, no manual configuration needed to run the default setup
2. dont abstract the actual compiler away
3. convention over configuration

### 1. asap?
The idea is that this should get you going as fast as possible (literally run the 'new' command and you are ready to go).\
But also if you are just creating a basic project, the tool should keep all the manual configuration away.

This is not intended for large and complex projects, if you plan to do such a thing, dont use ASAP.\
But you can start with ASAP and later switch, that should be straight forward.

### 2. dont abstract the compiler
The compiler and the way you configure it should not be abstracted away. You configure the flags exactly like you would for the compiler you choose.\
I.e. no special syntax and such. The only thing the configuration allows is to configure for which ASAP command which flags should be used.\

### 3. convention over configuration
This applies mainly to testing and the 'lib' directory.\
Files in the 'tests' folder that start with the name 'test_' are considered a test-suit and when using the 'test' command each of them will be run separately.

Next the 'lib' directory. By the default configuration the compiler is informed that it exists and it includes the sources in therefor compilation of the project and the tests.\
Its intended for header-only libraries or other libraries you want to vendor, not your code. However, you can do whatever you want.

## usage
to get going with a new project run `asap new my_project`. This will create a directory with the project name and set up a basic project scaffolding.\
The scaffolding includes the required directories 'lib','build','src' and 'tests' as well as some example files and the '.asap' configuration.\
The '.asap' configuration contains all the default configurations that will be used if they are not explicitly configured otherwise.\
I.e. if you remove a line, the default configuration for that will be used anyways.

The 'src' directory contains a basic main file with some 'hello-world' like program. You can run that simply with 'asap run'.\
The 'tests' directory contains an example test setup which again, you can run with 'asap test'.
The test command will run each test suit separately, i.e. compile all 'src' and that one `test_*` file and run it.\
For test utilities, name them `testutil_*`, then the tool will automatically include them in the compilation for the test code.


For the rest, just look at 'asap --help' to see what you can do.


## required dependencies
All required tools are only necessary if you run the command that need them. Without clang (or other compiler and configuration changes) most commands wont work.

The tool assumes you ...
* ... have clang with sufficient version to compile c++17 (configurable)
* ... have c++17 standard compliant libraries (if c++17 is used)
* ... have pkg-config to resolve dependencies (include command only)
* ... can run clang-tidy (check command only)
* ... have ccache (not required but recommended)
