const fs = require('fs')
const path = require('path')
const { execSync } = require('child_process')
const chalk = require('chalk')
const semver = require('semver')

const excludeFolder = ['node_modules']

function getAllTests() {
  return fs
    .readdirSync('./')
    .filter(i => {
      return (
        !i.startsWith('.') &&
        fs.statSync(i).isDirectory() &&
        !excludeFolder.includes(i)
      )
    })
    .map(i => {
      const p = path.join(__dirname, i)
      const tests = fs
        .readdirSync(p)
        .filter(j => fs.statSync(path.join(p, j)).isDirectory())
        .map(j => path.join(p, j))
      return tests
    })
}

getAllTests().map(tests => {
  tests.map(i => {
    console.log(chalk.green(`testing: ${i}`))
    const p = require(path.join(i, 'package.json'))
    if (p.engines && p.engines.node) {
      const currentNodeVersion = process.versions.node
      const range = p.engines.node
      const engineOk = semver.satisfies(currentNodeVersion, range)
      if (!engineOk) {
        console.warn(
          chalk.yellow(`${i} require Node.js ${range}, current is ${currentNodeVersion}, skipping`)
        )
        return
      }
    }
    const stdout = execSync('npm install', {
      cwd: i
    })
    console.log(stdout.toString())
  })
})
