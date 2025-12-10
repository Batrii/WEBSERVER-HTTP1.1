#include <parseArgument.hpp>

static void helpMessage() {
  console.log("Available options:");
  console.info("  --help            Display this help message");
  console.info("  --version         Display version information");
  console.info("  --auto-config     Automatically generate a configuration file for your web app");
  console.info("  `PATH/file.json`  Specify path to configuration file for your web app");
}

static void versionMessage() {
  console.log("Version 1.0.0");
  console.info("`abdallahelmadi42@gmail.com` for more informations");
}

int parseArgument(char* argument, clock_tt startClock) throw() {
  if (std::string(argument) == "--help") {
    helpMessage();
    return 0;
  }
  else if (std::string(argument) == "--version") {
    versionMessage();
    return 0;
  }
  else if (std::string(argument) == "--auto-config") {
    try {
      autoConfig(startClock);
      return 0;
    } catch (std::exception const& e) {
      console.issue("Failed to generate configuration file: " + std::string(e.what()));
      return 1;
    }
  } else {
    try {
      pathConfig(argument, startClock);
      return 0;
    } catch (std::exception const& e) {
      console.issue("Failed to load configuration file: " + std::string(e.what()));
      return 1;
    }
  }
}