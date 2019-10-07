#!/bin/sh

firstLine="const char MAIN_page[] PROGMEM = R\"=====("
value=$(cat index.html)
lastLine=")=====\";"

echo "${firstLine}\n${value}\n${lastLine}" > index.h
