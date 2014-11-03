# SQLTap

This is an SQLTap written in C++11, based on the Scala version.

### Query Syntax

```
Input ::= Query+

Query          ::= MODEL '.' FUNCTION
                   ['(' QueryArgs ')']
                    '{' FieldSpec '}'
                   ['&' Attribs] ';'
QueryArgs      ::= QueryArg (',' QueryArg)*
QueryArg       ::= '*'
                 | '$'
                 | NUMBER
                 | STRING
QueryAttribs   ::= 'for' '=' (ObjectID [',' ObjectID])

FieldSpec      ::= Field (',' Field)*
Field          ::= '*'
                 | FieldName
                 | DependantQuery

DependantQuery ::= Query

ObjectID       ::= NUMBER

MODEL          ::= <table name>
FUNCTION       ::= 'findOne'
                 | 'findAll'
                 | 'findAllWhere'
```
