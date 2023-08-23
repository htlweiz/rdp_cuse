#include <criterion/criterion.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <stdexcept>

#include <rdp_cuse.h>

Test(rdp_value, passing) {
  cr_assert(1);
}

Test(rdp_value, test_class) {
  auto now = std::time(nullptr);

  // std::clog << "NOW:    " << now << std::endl;
  auto gm_now(std::localtime(&now));
  // std::clog << "GM NOW: " << now << " " << std::asctime(gm_now);
  gm_now->tm_sec=0;
  gm_now->tm_min=0;
  gm_now->tm_hour=0;
  // std::clog << "GM TOD: " << mktime(gm_now) << " " << std::asctime(gm_now);
  auto yesterday = mktime(gm_now)- (24*60*60);
  auto gm_yesterday(std::localtime(&yesterday));
  // std::clog << "HM YTD: " << mktime(gm_yesterday) << " " << std::asctime(gm_yesterday);

  // real test starts here

  const time_t test_now=1690816942;
  const time_t expected_start=1690668000;
  const time_t expected_end=1690754400;

  // std::clog << "Test now is:       " << test_now   << " "     << std::asctime(std::localtime(&test_now));
  // std::clog << "Expected start is: " << expected_start << " " << std::asctime(std::localtime(&expected_start));
  // std::clog << "Expected end is:   " << expected_end   << " " << std::asctime(std::localtime(&expected_end));

  rdp::RdpValues rdp_values(test_now);
  // rdp::RdpValues rdp_values(now);

  // std::clog << rdp_values.start_time() << " " << rdp_values.end_time() << std::endl;

  cr_assert(rdp_values.start_time()==expected_start);
  cr_assert(rdp_values.end_time()==expected_end);

}

Test(rdp_value, test_out_of_bounds) {
  const time_t test_now=1690816942;
  const time_t expected_start=1690668000;
  const time_t expected_end=1690754400;
  rdp::RdpValues rdp_values(test_now);
  try {
    float hum = rdp_values.get_hum(expected_start-1);
    cr_assert(0); // Shoud throw an exception
  } catch (std::out_of_range e) {
    cr_assert(1); // this is the expected exception
  } catch (std::exception e) {
    cr_assert(0); // wrong exception
  } 
}

Test(rdp_value, test_rdp_value_types_enum) {
  cr_assert(rdp::RDP_NUM_VALUE_TYPES==3);
  cr_assert(rdp::RDP_TEMP_VALUE_TYPE==0);
  cr_assert(rdp::RDP_HUM_VALUE_TYPE==1);
  cr_assert(rdp::RDP_PRESS_VALUE_TYPE==2);
}

Test(rdp_value, test_rdp_value) { 
  rdp::RdpValue the_value(rdp::RDP_HUM_VALUE_TYPE, 0, 0.1);
  cr_assert(the_value.get_type()==rdp::RDP_HUM_VALUE_TYPE);
  cr_assert(the_value.get_time()==0);
  float diff = the_value.get_value()-0.1;
  cr_assert(fabs(diff)<0.00000001);
}

Test(rdp_value, test_rdp_value_store) {
  rdp::RdpValueStore the_store(1000);
  the_store.add_value(rdp::RDP_TEMP_VALUE_TYPE, 0, 0.4);
  the_store.add_value(rdp::RDP_TEMP_VALUE_TYPE, 1, 0.5);
  the_store.add_value(rdp::RDP_TEMP_VALUE_TYPE, 2, 0.6);
  
  auto value1 = the_store.get_value();
  cr_assert(value1.get_type()==rdp::RDP_TEMP_VALUE_TYPE);
  // std::clog << "Timestamp: " << value1.get_time() << std::endl;
  cr_assert(value1.get_time()==1000);
  float diff = value1.get_value()-0.4;
  // std::clog << "Diff: " << diff << std::endl;
  cr_assert(fabs(diff)<0.0000001);

  auto value2 = the_store.get_value();
  cr_assert(value2.get_type()==rdp::RDP_TEMP_VALUE_TYPE);
  // std::clog << "Timestamp: " << value2.get_time() << std::endl;
  cr_assert(value2.get_time()==1001);
  diff = value2.get_value()-0.5;
  // std::clog << "Diff: " << diff << std::endl;
  cr_assert(fabs(diff)<0.0000001);
  
  auto value3 = the_store.get_value();
  cr_assert(value3.get_type()==rdp::RDP_TEMP_VALUE_TYPE);
  // std::clog << "Timestamp: " << value3.get_time() << std::endl;
  cr_assert(value3.get_time()==1002);
  diff = value3.get_value()-0.6;
  // std::clog << "Diff: " << diff << std::endl;
  cr_assert(fabs(diff)<0.0000001);
  
  auto value4 = the_store.get_value();
  cr_assert(value4.get_type()==rdp::RDP_TEMP_VALUE_TYPE);
  // std::clog << "Timestamp: " << value4.get_time() << std::endl;
  cr_assert(value4.get_time()==1000);
  diff = value4.get_value()-0.4;
  //  std::clog << "Diff: " << diff << std::endl;
  cr_assert(fabs(diff)<0.0000001);
}