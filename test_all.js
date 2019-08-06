const fs = require('fs')
const path = require('path')
const { execSync } = require('child_process')

fs.readdirSync("./")
  .filter(i => {
    return !i.startsWith(".") && fs.statSync(i).isDirectory()
  })
  .map(i => {
    const p = path.join(__dirname, i)
    const tests = fs.readdirSync(p)
      .filter(j => fs.statSync(path.join(p, j)).isDirectory())
      .map(j => path.join(p, j))
    return tests
  })
  .map(tests => {
    tests.map(i => {
      console.log(`testing`, i);
      const stdout = execSync('npm install', {
        cwd: i
      }
      console.log(stdout.toString())
    })
  })
