# Call Stack Spoofer

## Overview
Call Stack Spoofer is a Windows-based tool that modifies the call stack of a suspended thread by injecting user-specified memory addresses. This is useful for stack spoofing, debugging, and research on process execution flow manipulation.

## Features
- Accepts a list of memory addresses as arguments.
- Suspends a thread and modifies its stack.
- Resumes execution with the modified stack.
- Includes a help option (`-h`) for guidance.

## Usage
### **Command Syntax**
```
CallStackSpoofer.exe <addr1> <addr2> ...
```
### **Example**

```
CallStackSpoofer.exe 00007FFEDD85B8E0 00007FFEDD85B8E1 00007FFEDD85B85
```

### **Help Option**
To display usage instructions, run:

```
CallStackSpoofer.exe -h
```

## Compilation
To compile the source code, use **MinGW** or **Microsoft Visual Studio**:

### **Using MinGW**
```
g++ -o CallStackSpoofer.exe source.cpp -static -mwindows
```

## Compilation
To compile the source code, use **MinGW** or **Microsoft Visual Studio**:

### **Using MinGW**
