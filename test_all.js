const fs = require("fs");
const path = require("path");
const { execSync } = require("child_process");
const chalk = require("chalk");
const semver = require("semver");

const excludeFolder = ["node_modules", "website"];

function getAllExamples(pathToCheck) {
  const directoriesToTest = [];
  for (const fd of fs.readdirSync(pathToCheck)) {
    if (excludeFolder.includes(fd)) {
      continue;
    }
    const absPath = path.join(pathToCheck, fd);
    if (fs.statSync(absPath).isDirectory()) {
      directoriesToTest.push(...getAllExamples(absPath));
    }

    if (fs.existsSync(path.join(absPath, "package.json"))) {
      directoriesToTest.push(absPath);
    }
  }
  return directoriesToTest;
}

const passed = [];
const failedInstalls = [];
const noTest = [];
const failedTests = [];
for (directoryToTest of getAllExamples(path.join(__dirname))) {
  console.log(chalk.green(`testing: ${directoryToTest}`));
  const pkgJson = require(path.join(directoryToTest, "package.json"));
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

  try {
    const stdout = execSync("npm install", { cwd: directoryToTest });
    console.log(stdout.toString());
  } catch (err) {
    console.log(err);
    failedInstalls.push(directoryToTest);
    continue;
  }

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
