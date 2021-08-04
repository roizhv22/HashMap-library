# C programing workshop final exercise - Hash map implementation (include vector's implementation and tests).

In this final exercise we were required to implement a generic hash map static library with all the needed features, to support the chaining procedure (each cell in the table is a vector of key-value pairs – simple struct). 

This is dynamically allocated hash map and vector. During the insertion operations the table will change it's size in order to maintain the required performance to support the O(1) (in average) operations, thus keeping the load factor as needed by a given threshold.

There is also a feature of apply_if – if a given key fulfills a given condition, a function will be called on its value. 

We were also required to write a full test suite for the project, I ran some tests with different key-value types and large inputs. 
As you may observe I written a makefile to compile and create the library with ease as well. 
