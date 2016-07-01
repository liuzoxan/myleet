/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef tutorial_TYPES_H
#define tutorial_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>
#include "shared_types.h"


namespace tutorial {

struct Operation {
  enum type {
    ADD = 1,
    SUBTRACT = 2,
    MULTIPLY = 3,
    DIVIDE = 4
  };
};

extern const std::map<int, const char*> _Operation_VALUES_TO_NAMES;

typedef int32_t MyInteger;

typedef struct _Work__isset {
  _Work__isset() : num1(true), num2(false), op(false), comment(false) {}
  bool num1;
  bool num2;
  bool op;
  bool comment;
} _Work__isset;

class Work {
 public:

  static const char* ascii_fingerprint; // = "928D5DB19D957BA73E98532DF590DAF5";
  static const uint8_t binary_fingerprint[16]; // = {0x92,0x8D,0x5D,0xB1,0x9D,0x95,0x7B,0xA7,0x3E,0x98,0x53,0x2D,0xF5,0x90,0xDA,0xF5};

  Work() : num1(0), num2(0), op((Operation::type)0), comment() {
  }

  virtual ~Work() throw() {}

  int32_t num1;
  int32_t num2;
  Operation::type op;
  std::string comment;

  _Work__isset __isset;

  void __set_num1(const int32_t val) {
    num1 = val;
  }

  void __set_num2(const int32_t val) {
    num2 = val;
  }

  void __set_op(const Operation::type val) {
    op = val;
  }

  void __set_comment(const std::string& val) {
    comment = val;
    __isset.comment = true;
  }

  bool operator == (const Work & rhs) const
  {
    if (!(num1 == rhs.num1))
      return false;
    if (!(num2 == rhs.num2))
      return false;
    if (!(op == rhs.op))
      return false;
    if (__isset.comment != rhs.__isset.comment)
      return false;
    else if (__isset.comment && !(comment == rhs.comment))
      return false;
    return true;
  }
  bool operator != (const Work &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Work & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Work &a, Work &b);

typedef struct _InvalidOperation__isset {
  _InvalidOperation__isset() : whatOp(false), why(false) {}
  bool whatOp;
  bool why;
} _InvalidOperation__isset;

class InvalidOperation : public ::apache::thrift::TException {
 public:

  static const char* ascii_fingerprint; // = "3F5FC93B338687BC7235B1AB103F47B3";
  static const uint8_t binary_fingerprint[16]; // = {0x3F,0x5F,0xC9,0x3B,0x33,0x86,0x87,0xBC,0x72,0x35,0xB1,0xAB,0x10,0x3F,0x47,0xB3};

  InvalidOperation() : whatOp(0), why() {
  }

  virtual ~InvalidOperation() throw() {}

  int32_t whatOp;
  std::string why;

  _InvalidOperation__isset __isset;

  void __set_whatOp(const int32_t val) {
    whatOp = val;
  }

  void __set_why(const std::string& val) {
    why = val;
  }

  bool operator == (const InvalidOperation & rhs) const
  {
    if (!(whatOp == rhs.whatOp))
      return false;
    if (!(why == rhs.why))
      return false;
    return true;
  }
  bool operator != (const InvalidOperation &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const InvalidOperation & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(InvalidOperation &a, InvalidOperation &b);

} // namespace

#endif
