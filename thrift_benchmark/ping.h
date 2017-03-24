/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Something_H
#define Something_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "ping_types.h"

namespace Test {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class SomethingIf {
 public:
  virtual ~SomethingIf() {}
  virtual int32_t ping() = 0;
};

class SomethingIfFactory {
 public:
  typedef SomethingIf Handler;

  virtual ~SomethingIfFactory() {}

  virtual SomethingIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(SomethingIf* /* handler */) = 0;
};

class SomethingIfSingletonFactory : virtual public SomethingIfFactory {
 public:
  SomethingIfSingletonFactory(const boost::shared_ptr<SomethingIf>& iface) : iface_(iface) {}
  virtual ~SomethingIfSingletonFactory() {}

  virtual SomethingIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(SomethingIf* /* handler */) {}

 protected:
  boost::shared_ptr<SomethingIf> iface_;
};

class SomethingNull : virtual public SomethingIf {
 public:
  virtual ~SomethingNull() {}
  int32_t ping() {
    int32_t _return = 0;
    return _return;
  }
};


class Something_ping_args {
 public:

  Something_ping_args(const Something_ping_args&);
  Something_ping_args& operator=(const Something_ping_args&);
  Something_ping_args() {
  }

  virtual ~Something_ping_args() throw();

  bool operator == (const Something_ping_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Something_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Something_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Something_ping_pargs {
 public:


  virtual ~Something_ping_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Something_ping_result__isset {
  _Something_ping_result__isset() : success(false) {}
  bool success :1;
} _Something_ping_result__isset;

class Something_ping_result {
 public:

  Something_ping_result(const Something_ping_result&);
  Something_ping_result& operator=(const Something_ping_result&);
  Something_ping_result() : success(0) {
  }

  virtual ~Something_ping_result() throw();
  int32_t success;

  _Something_ping_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const Something_ping_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Something_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Something_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Something_ping_presult__isset {
  _Something_ping_presult__isset() : success(false) {}
  bool success :1;
} _Something_ping_presult__isset;

class Something_ping_presult {
 public:


  virtual ~Something_ping_presult() throw();
  int32_t* success;

  _Something_ping_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class SomethingClient : virtual public SomethingIf {
 public:
  SomethingClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  SomethingClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  int32_t ping();
  void send_ping();
  int32_t recv_ping();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class SomethingProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<SomethingIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (SomethingProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  SomethingProcessor(boost::shared_ptr<SomethingIf> iface) :
    iface_(iface) {
    processMap_["ping"] = &SomethingProcessor::process_ping;
  }

  virtual ~SomethingProcessor() {}
};

class SomethingProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  SomethingProcessorFactory(const ::boost::shared_ptr< SomethingIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< SomethingIfFactory > handlerFactory_;
};

class SomethingMultiface : virtual public SomethingIf {
 public:
  SomethingMultiface(std::vector<boost::shared_ptr<SomethingIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~SomethingMultiface() {}
 protected:
  std::vector<boost::shared_ptr<SomethingIf> > ifaces_;
  SomethingMultiface() {}
  void add(boost::shared_ptr<SomethingIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int32_t ping() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping();
    }
    return ifaces_[i]->ping();
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class SomethingConcurrentClient : virtual public SomethingIf {
 public:
  SomethingConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  SomethingConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  int32_t ping();
  int32_t send_ping();
  int32_t recv_ping(const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

} // namespace

#endif
