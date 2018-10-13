## Contributing to Menrva

# Requirements
  - Android Studio v3+
  - Android SDK 21
  - Android NDK
  - Java SDK (JDK)
  
## Tools & Tips

# javah
A tool provided with the JDK for generating header files.  We will be using it to generate C++ JNI Header files.

# Integrate javah into Android Studio
  - Open File -> Settings -> Tools -> External Tools
  - Add a new External Tools
  - Use the following settings to configure the External Tool Parameters
    * Program : $JDKPath$\bin\javah.exe
    * Parameters : -classpath "$Classpath$" -v -jni "$FileClass$"
    * Working Directory : $ProjectFileDir$\app\src\main\cpp\jni

# Using javah to Generate a JNI Header File
  - Create a Java Class which declares a method using the 'native' keyword
  - Right-click on the Java Class -> External Tools -> javah
  - Use the newly created C++ Header File in app\main\cpp\jni to expose the necessary native methods
