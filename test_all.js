const fs = require("fs");
const path = require("path");
const { execSync } = require("child_process");
const semver = require("semver");

const examplesFolder = path.join(__dirname, "src");

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

const main = async () => {
  const { default: chalk } = await import("chalk");
  const passed = [];
  const failedBuilds = [];
  const noTest = [];
  const failedTests = [];
  for (directoryToTest of getAllExamples(examplesFolder)) {
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

    let buildCommand = "npx node-gyp rebuild";
    if ("scripts" in pkgJson && "install" in pkgJson.scripts) {
      buildCommand = "npm run install";
    }
    try {
      const stdout = execSync(buildCommand, { cwd: directoryToTest });
      console.log(stdout.toString());
    } catch (err) {
      console.log(err);
      failedBuilds.push(directoryToTest);
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

  if (failedBuilds.length > 0) {
    console.error(chalk.red("failed to build:"));
    failedBuilds.map((dir) => console.warn(chalk.red(`    ${dir}`)));
  }
  if (failedTests.length > 0) {
    console.error(chalk.red("failed tests:"));
    failedTests.map((dir) => console.warn(chalk.red(`    ${dir}`)));
  }
};

main().catch(e => {
  console.error(e);
  process.exitCode = 1;
});
