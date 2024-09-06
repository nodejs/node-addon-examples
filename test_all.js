const fs = require("fs");
const path = require("path");
const { execSync } = require("child_process");
const chalk = require("chalk");
const semver = require("semver");

const examplesFolder = path.join(__dirname, "src");

/**
 * Recursively get all folders with package.json
 */
function getAllExamples(pathToCheck) {
  const directoriesToTest = [];
  for (const fd of fs.readdirSync(pathToCheck)) {
    const absPath = path.join(pathToCheck, fd);
    if (fs.existsSync(path.join(absPath, "package.json"))) {
      directoriesToTest.push(absPath);
      continue;
    }
    if (fs.statSync(absPath).isDirectory()) {
      directoriesToTest.push(...getAllExamples(absPath));
    }
  }
  return directoriesToTest;
}

const passed = [];
const failedInstalls = [];
const noTest = [];
const failedTests = [];
for (directoryToTest of getAllExamples(examplesFolder)) {
  console.log(chalk.green(`testing: ${directoryToTest}`));
  const pkgJson = require(path.join(directoryToTest, "package.json"));

  // Check if a node version exists, then skip the project if the current node version does not satisfy the range
  if (pkgJson.engines && pkgJson.engines.node) {
    const currentNodeVersion = process.versions.node;
    const range = pkgJson.engines.node;
    const engineOk = semver.satisfies(currentNodeVersion, range);
    if (!engineOk) {
      console.warn(
        chalk.yellow(
          `${directoryToTest} require Node.js ${range}, current is ${currentNodeVersion}, skipping`
        )
      );
      continue;
    }
  }

  // install dependencies
  try {
    const stdout = execSync("npm install", { cwd: directoryToTest });
    console.log(stdout.toString());
  } catch (err) {
    console.log(err);
    failedInstalls.push(directoryToTest);
    continue;
  }

  // choose a project entry point scripts.start or scripts.test or main file
  let testCommand;
  if ("scripts" in pkgJson && "start" in pkgJson.scripts) {
    testCommand = "npm start";
  } else if ("scripts" in pkgJson && "test" in pkgJson.scripts) {
    testCommand = "npm test";
  } else if ("main" in pkgJson) {
    testCommand = `node ${pkgJson.main}`
  } else {
    noTest.push(directoryToTest);
    continue;
  }

  // call the test command
  try {
    const stdout = execSync(testCommand, { cwd: directoryToTest });
    console.log(stdout.toString());
    passed.push(directoryToTest);
  } catch (err) {
    console.log(err);
    failedTests.push(directoryToTest);
  }
}

passed.map((dir) => console.log(chalk.green(`passed: ${dir}`)));

if (noTest.length > 0) {
  console.warn(chalk.yellow("no test found:"));
  noTest.map((dir) => console.warn(chalk.yellow(`    ${dir}`)));
}

if (failedInstalls.length > 0) {
  console.error(chalk.red("failed to install:"));
  failedInstalls.map((dir) => console.warn(chalk.red(`    ${dir}`)));
}
if (failedTests.length > 0) {
  console.error(chalk.red("failed tests:"));
  failedTests.map((dir) => console.warn(chalk.red(`    ${dir}`)));
}
