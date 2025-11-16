# imports
import os;
import sys;
import shutil;

# build of gbe
class GBEBuild:
    #constructor
    def __init__(self, debug: bool = False, test: bool = False, coverage: bool = False):
        # init members
        self.__buildDirectory = ""
        self.__buildStatus = 0
        self.__buildErrorMessage = "OK"
        self.__coverage = coverage;
    
        # build directory
        self.__buildDirectory = GBEBuild.__createBuilDirectory(debug, test)
        self.__executable = f'{self.__buildDirectory}/gbe';

        # create build directory
        if not os.path.isdir(self.__buildDirectory):
            os.makedirs(self.__buildDirectory)

        # copy the assets folder
        shutil.copytree("./assets", self.__buildDirectory, dirs_exist_ok=True)

        # change to buildDirectory
        workingDirectory = os.getcwd()
        os.chdir(self.__buildDirectory)

        # cmake    
        cmakeCommand = ["cmake", workingDirectory]

        # check debug / release
        cmakeBuildType = "Debug" if debug else "Release"
        cmakeCommand.append(f"-DCMAKE_BUILD_TYPE={cmakeBuildType}")

        # check if test is enabled
        if test:
            cmakeCommand.append("-DTEST=ON")

        if coverage: 
            cmakeCommand.append("-DCOVERAGE=ON")

        err = os.system(' '.join(cmakeCommand))
        if err:
            self.__setError(err, "CMake failed")
            return

        # remove old executable
        self.__executable = f'{self.__buildDirectory}/gbe'
        if os.path.isfile(self.__executable):
            os.remove(self.__executable)

        # make 
        err = os.system("make");
        if err:
            self.__setError(err, "Make failed")
            return

        # go back to working directory
        os.chdir(workingDirectory)

    # run created build
    def run(self, arguments: list[str]) -> int:
        runCommand = []

        if coverage:
            runCommand = [f"ctest -T test --test-dir {self.__buildDirectory}"]
            runCommand.append("&&")
            runCommand.append(f"ctest -T coverage --test-dir {self.__buildDirectory}")
        else:
            os.chdir(self.__buildDirectory);
            runCommand = ["./gbe"] + arguments

        return os.system(' '.join(runCommand))
    
    # get build status
    def getBuildStatus(self) -> int:
        return self.__buildStatus

    # get build error message
    def getBuildErrorMessage(self) -> str:
        return self.__buildErrorMessage

    # set error
    def __setError(self, status: int, message: str) -> None:
        self.__buildStatus = status
        self.__buildErrorMessage = f'[ERROR {status}] -> {message}'

    # create build directory
    def __createBuilDirectory(debug: bool = False, test: bool = False) -> str:
        # build directory
        buildDiractoryBuilder = [".", ".build"]
        
        buildDiractoryBuilder.append("test" if test else "desktop")
        buildDiractoryBuilder.append("debug" if debug else "release")
        
        buildDirectory = '/'.join(buildDiractoryBuilder)

        return buildDirectory;


# check flag in argument
def checkFlag(flag: str, arguments: list[str]) -> bool:
    if flag in arguments:
        arguments.remove(flag)
        return True

    return False

# main
if __name__ == "__main__":
    arguments = sys.argv[1:]
    
    # check test flag
    test = checkFlag("--test", arguments)

    # check debug flag
    debug = checkFlag("--debug", arguments)

    # check coverage flag
    coverage = checkFlag("--coverage", arguments)
    if coverage:
        debug = True
        test = True

    # create the build
    gbeBuild = GBEBuild(debug, test, coverage)
    
    # check status
    if gbeBuild.getBuildStatus():
        print(gbeBuild.getBuildErrorMessage)
        exit(gbeBuild.getBuildStatus())

    # run
    print(arguments)
    exit(gbeBuild.run(arguments))
    

