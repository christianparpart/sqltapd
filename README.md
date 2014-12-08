# SQLTap

This is an SQLTap written in C++11, based on the Scala version.

### How to Build from Sources

```
apt-get install clang++-3.4
git clone git@github.com:trapni/sqltapd.git
cd sqltapd
make debug
```

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

### TODO

- [x] lexer
- [x] parser
- [x] AST printer
- [x] Manifest API
- [ ] Manifest: schema.xml parser
- [ ] query executor (serialized)
- [ ] query executor (paralized)
- [ ] cache API
- [ ] cache API: memcache backend
- [ ] HTTP: query API
- [ ] HTTP: cache invalidation API
- [ ] mysql slave replication listener for cache invalidation
- [ ] unit tests
- [ ] native C extension binding (gem for Ruby)

### Open Questions

- what is `base_score` in schema.xml
- schema.xml: field.type seems dead code, not really used.
