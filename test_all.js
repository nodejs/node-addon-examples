const fs = require("fs");
const path = require("path");
const { execSync } = require("child_process");
const chalk = require("chalk");
const semver = require("semver");

const excludeFolder = ["node_modules", "website"];

function getAllTests(pathToCheck) {
  const directoriesToTest = [];
  for (const fd of fs.readdirSync(pathToCheck)) {
    if (excludeFolder.includes(fd)) {
      continue;
    }
    const absPath = path.join(pathToCheck, fd);
    if (fs.statSync(absPath).isDirectory()) {
      directoriesToTest.push(...getAllTests(absPath));
    }

    if (fs.existsSync(path.join(absPath, "package.json"))) {
      directoriesToTest.push(absPath);
    }
  }
  return directoriesToTest;
}

for (directoryToTest of getAllTests(__dirname)) {
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
  let stdout = execSync("npm install", { cwd: directoryToTest });
  console.log(stdout.toString());
  if ("test" in pkgJson.scripts) {
    stdout = execSync("npm test", { cwd: directoryToTest });
    console.log(stdout.toString());
  }
}
