/*class TestEventTime
 *  
 *  Class to study Event Time
 *
 */
 
#include "DataFormats/Provenance/interface/Timestamp.h"
#include "TestEventTime.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time.hpp"

using namespace edm;

TestEventTime::TestEventTime(const edm::ParameterSet& iPSet)  
{    
}

TestEventTime::~TestEventTime() {}

void TestEventTime::beginJob()
{
}

void TestEventTime::endJob(){return;}
void TestEventTime::beginRun(const edm::Run& iRun,const edm::EventSetup& iSetup)
{
}
void TestEventTime::endRun(const edm::Run& iRun,const edm::EventSetup& iSetup){return;}
void TestEventTime::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
{ 

  const edm::Timestamp time = iEvent.time ();
  timeval eventTime;
  eventTime.tv_sec = time.value () >> 32;
  eventTime.tv_usec = 0xFFFFFFFF & time.value ();
  boost::posix_time::ptime bt0 = boost::posix_time::from_time_t(0);
  boost::posix_time::ptime bt = bt0 +
    boost::posix_time::seconds(eventTime.tv_sec)
    + boost::posix_time::microseconds(eventTime.tv_usec);

  std::cout << "Event time: unixTime = " << time.unixTime() << " value = " << time.value() << " formatted = " << bt << std::endl;

}//analyze

