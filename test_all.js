const fs = require('fs')
const path = require('path')
const { exec } = require('child_process')

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
      console.log(i);
      exec('npm', {
        cwd: i
      }, (error, stdout, stderr) => {
        if (error) {
          console.error(`failed with ${i} with error`, error)
          process.exit(-1)
        }
        console.log(stdout.toString() + stderr.toString())
      })
    })
  })
