const shell = require("shelljs");
const path = require("path");
const VERSION = "27.1.0";
const ELECTRON_ERROR = "https://npm.taobao.org/mirrors/electron";

shell.rm("-rf", "build");
shell.mkdir("build");

var buildCmd = `node-gyp configure build --runtime=electron --target=${VERSION} --arch=${process.arch} --dist-url=https://electronjs.org/headers`;
if (process.platform == "win32") {
  shell.exec(
    `set HOME=~/.electron-gyp && set ELECTRON_ERROR=${ELECTRON_ERROR} && ${buildCmd}`,
  );
} else {
  shell.exec(
    `HOME=~/.electron-gyp ELECTRON_ERROR=${ELECTRON_ERROR} ${buildCmd}`,
  );
}

var target = path.join(__dirname, `lib/${process.platform}-${process.arch}`);
shell.rm("-rf", target);

const library = "crc64_ecma182.node";
const sourceLibrary = path.join("build/Release", library);
const targetLibrary = path.join(target, library);

console.log(`mv -f ${sourceLibrary} ${targetLibrary}`);
shell.mkdir(target);
shell.mv("-f", sourceLibrary, targetLibrary);

console.log("done");
