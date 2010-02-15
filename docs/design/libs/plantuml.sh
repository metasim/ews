#/bin/bash

# Wherever we are, use parent dir to this script as working dir.
cd `dirname $0`/..

#exec java -jar libs/plantuml.jar -config libs/plantuml.cfg -v -keepfiles $*
exec java -jar libs/plantuml.jar -config libs/plantuml.cfg  $*
