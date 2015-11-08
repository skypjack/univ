#!/bin/bash

CLASSPATH=$CLASSPATH:server/nnsecserver.jar:client/nnsecclient.jar

# È possibile anche usare la classe Properties, nel codice, in realtà

echo""
echo "launching server ..."

java -Djava.rmi.server.hostname=blackcube -Djava.security.policy=server/policy -Djavax.net.ssl.keyStore=server/keystore -Djavax.net.ssl.keyStorePassword=nnsecks nnsec.server.PocketServer &

echo ""
echo "launching client ..."

java -Djava.rmi.server.hostname=192.168.11.106 -Djavax.net.ssl.trustStore=client/truststore -Djavax.net.ssl.trustStorePassword=nnsects nnsec.client.Client &
