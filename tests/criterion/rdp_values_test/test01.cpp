#include <criterion/criterion.h>

#include <iostream>

#include <rdp_cuse.h>

Test(factory, passing) {
  cr_assert(1);
  rdp::RdpValueStore the_store = rdp::RdpValueFactory::create(0,"../tests/criterion/rdp_values_test/test_data.csv");
  // std::cout << "HI: " << &the_store << std::endl;
  auto one_value = the_store.get_value();
  cr_assert(one_value.get_time() == 0);
  cr_assert(one_value.get_type() == rdp::RDP_TEMP_VALUE_TYPE);
  cr_assert(one_value.get_value() < 1000);
  auto this_type = one_value.get_type();
  auto this_time = one_value.get_time();
  auto this_value = one_value.get_value();
  auto two_value = the_store.get_value();
  auto other_type = two_value.get_type();
  auto other_time = two_value.get_time();
  auto other_value = two_value.get_value(); 
  cr_assert( (this_time != other_time) ||
      ( this_type != other_type ) || 
      ( this_value != other_value) );
}
