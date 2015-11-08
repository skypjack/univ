#!/bin/bash

echo ""
echo -n "compiling source code "

echo -n "."
javac nnsec/net/*.java
echo -n "."
javac nnsec/parser/*.java
echo -n "."
javac nnsec/cryptosystem/*.java
echo -n "."
javac nnsec/client/Calculator.java
echo -n "."
rmic nnsec.client.Calculator
echo -n "."
javac nnsec/server/Worker.java
echo -n "."
rmic nnsec.server.Worker
echo -n "."
javac nnsec/server/WorkerFactory.java
echo -n "."
rmic nnsec.server.WorkerFactory
echo -n "."
javac nnsec/server/*.java
echo -n "."
javac nnsec/client/*.java
echo "."

echo ""
echo -n "making nnsec doc "

echo -n "."
javadoc -quiet -d doc/ nnsec nnsec.server nnsec.client nnsec.cryptosystem nnsec.parser nnsec.net
echo "."

echo ""
echo -n "making .jar files "

echo -n "."
jar -cfm server/nnsecserver.jar server/smanifest.mf nnsec/server/*.class nnsec/client/Calculator_Stub.class nnsec/client/CalculatorIFace.class nnsec/cryptosystem/Cryptosystem\$PublicKey.class nnsec/cryptosystem/Cryptosystem.class nnsec/cryptosystem/Cryptosystem\$MessageTooBigException.class nnsec/cryptosystem/Mapper.class nnsec/parser/*.class nnsec/net/*.class JFlex/*.class java_cup/*.class java_cup/runtime/*.class
echo -n "."
jar -cfm client/nnsecclient.jar client/cmanifest.mf nnsec/client/*.class nnsec/cryptosystem/*.class nnsec/server/WorkerFactory_Stub.class nnsec/server/WorkerFactoryIFace.class nnsec/server/Worker_Stub.class nnsec/server/WorkerIFace.class
echo "."

echo ""
echo -n "making .tar.gz files "

echo -n "."
tar -czf package/nnsecserver.tar.gz server/nnsecserver.jar server/keystore server/nnsec.cer server/policy server/README net/*.net
echo -n "."
tar -czf package/nnsecclient.tar.gz client/nnsecclient.jar client/truststore client/nnsec.cer client/README
echo -n "."
tar -czf package/nnsecdoc.tar.gz doc/
echo -n "."
tar -czf package/nnsecfull.tar.gz package/nnsecserver.tar.gz package/nnsecclient.tar.gz package/nnsecdoc.tar.gz
echo -n "."
tar -czf package/nnsecsources.tar.gz nnsec/server/*.java nnsec/client/*.java nnsec/net/*.java nnsec/parser/*.java nnsec/cryptosystem/*.java
echo "."
