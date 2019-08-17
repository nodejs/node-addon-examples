sudo cp ./lib/libHelloWorld.so /usr/lib/ && \
sudo cp ./lib/libHelloWorld.h /usr/include/ && \
node-gyp configure build && \
sudo node module.js
exit 0
