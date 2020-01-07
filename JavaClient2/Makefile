JFLAGS =
JC = javac



.SUFFIXES: .java .class
.java.class:
	mkdir -p build
	$(JC) $(JFLAGS) $*.java

default:
	javac -d build -cp build src/*.java src/aquarium/* src/parser/* src/prompt/* src/fishnet/* src/starter/*

tests: test

test : default
	java -cp build Main

Main: default
	java  -ea -cp build/ Main

clean:
	find . -name *.class -exec rm -v {} \;
