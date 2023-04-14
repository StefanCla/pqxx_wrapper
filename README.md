# pqxx_wrapper
The pqxx_wrapper library is a library used to enscapsule commonly used functions to always return zero initialized variables and error prints.
This is handy for when you want to prevent your project from crashing whenever an incorrect SQL script is passed or other unexpected errors occur.
The library also allows users to enable asserts to be triggered for easier debugging.

### Disclaimer, this library is not endorsed or promoted by pqxx or their associatives.

### Road Map
- [X] Database connection
- [X] Transactions
- [X] Execute queries
- [X] Query single values
- [X] Query array values
