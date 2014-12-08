# SQLTap

This is an SQLTap written in C++11, based on the Scala version.

### How to Build from Sources

```
apt-get install clang++-3.4 libxml2-dev
git clone git@github.com:trapni/sqltapd.git
cd sqltapd
make debug
```

*Please note*, when running the server (in its WIP-test version), you'll
need a `schema.xml` in the project root directory.

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

### TODO: ms1

- [x] lexer
- [x] parser
- [x] AST printer
- [x] Manifest API
- [x] Manifest: schema.xml parser (loading resources)
- [ ] query executor (serialized)
- [ ] query executor (paralized, find out out query dependencies)
- [ ] HTTP: query API
- [ ] unit tests
- [ ] native C extension binding (gem for Ruby) (maybe optional)

### TODO: ms2

- [ ] cache API
- [ ] cache memcache backend
- [ ] ctree index loading out of schema.xml
- [ ] HTTP: cache invalidation API
- [ ] mysql slave replication listener for cache invalidation

### Open Questions

- what is `base_score` in schema.xml
- schema.xml: field.type seems dead code, not really used.
- currently the schema.xml loader is part of Manifest.cpp: move that out?

