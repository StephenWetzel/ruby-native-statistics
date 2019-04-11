#include "ruby.h"
#include "dispersion.h"

void Init_dispersion() {
  DispersionModule = rb_define_module("Dispersion");
  rb_define_method(DispersionModule, "stdev", rb_sample_standard_deviation, 0);
  rb_define_method(DispersionModule, "stdevs", rb_sample_standard_deviation, 0);
  rb_define_method(DispersionModule, "stdevp", rb_population_standard_deviation, 0);
  rb_define_method(DispersionModule, "var", rb_sample_variance, 0);
  rb_define_method(DispersionModule, "varp", rb_population_variance, 0);
  rb_define_method(DispersionModule, "mean", rb_mean, 0);
  rb_define_method(DispersionModule, "median", rb_median, 0);
}

double calculate_mean(VALUE array, unsigned long array_length) {
  unsigned long i;
  double total = 0;
  double mean = 0;

  for(i = 0; i < array_length; i++) {
    total += rb_num2dbl(rb_ary_entry(array, i));
  }

  mean = total / array_length;

  return mean;
}

double calculate_median(VALUE array, unsigned long array_length) {
  unsigned long n;
  sort_array(array, array_length);
  n = (array_length + 1) / 2 - 1;
  if (array_length % 2 == 0) {
    return rb_num2dbl((rb_ary_entry(array, n) + rb_ary_entry(array, n + 1)) / 2.0);
  }
  return rb_num2dbl(rb_ary_entry(array, n));
}

void sort_array(VALUE array, unsigned long array_length) {
  unsigned long ii, jj, temp;

  for(ii = 0; ii < array_length - 1; ii++) {
    for(jj = 0; jj < array_length - ii - 1; jj++) {
      if(rb_ary_entry(array, jj) > rb_ary_entry(array, jj + 1)) {
        temp = rb_ary_entry(array, jj);
        rb_ary_store(array, jj, rb_ary_entry(array, jj + 1));
        rb_ary_store(array, jj + 1, temp);
      }
    }
  }
}

double calculate_total_distance_from_mean(VALUE array, unsigned long array_length) {
  unsigned long i;
  double mean = 0;
  double total_distance_from_mean = 0;

  mean = calculate_mean(array, array_length);

  for(i = 0; i < array_length; i++){
    total_distance_from_mean += pow((rb_num2dbl(rb_ary_entry(array, i)) - mean), 2);
  }

  return total_distance_from_mean;
}

VALUE rb_sample_standard_deviation(VALUE self) {
  unsigned int array_length;

  Check_Type(self, T_ARRAY);

  array_length = rb_long2int(RARRAY_LEN(self));

  if (array_length <= 1) {
    rb_raise(rb_eRangeError, "array must have more than one element");
  }

  return DBL2NUM(sqrt((calculate_total_distance_from_mean(self, array_length)/(array_length - 1))));
}

VALUE rb_sample_variance(VALUE self) {
  unsigned int array_length;

  Check_Type(self, T_ARRAY);

  array_length = rb_long2int(RARRAY_LEN(self));

  if (array_length <= 1) {
    rb_raise(rb_eRangeError, "array must have more than one element");
  }

  return DBL2NUM((calculate_total_distance_from_mean(self, array_length)/(array_length - 1)));
}

VALUE rb_population_standard_deviation(VALUE self) {
  unsigned int array_length;

  Check_Type(self, T_ARRAY);

  array_length = rb_long2int(RARRAY_LEN(self));

  if (array_length <= 1) {
    rb_raise(rb_eRangeError, "array must have more than one element");
  }

  return DBL2NUM(sqrt(calculate_total_distance_from_mean(self, array_length) / array_length));
}

VALUE rb_population_variance(VALUE self) {
  unsigned int array_length;

  Check_Type(self, T_ARRAY);

  array_length = rb_long2int(RARRAY_LEN(self));

  if (array_length <= 1) {
    rb_raise(rb_eRangeError, "array must have more than one element");
  }

  return DBL2NUM(calculate_total_distance_from_mean(self, array_length) / array_length);
}

VALUE rb_mean(VALUE self) {
  unsigned int array_length;

  Check_Type(self, T_ARRAY);

  array_length = rb_long2int(RARRAY_LEN(self));

  if (array_length <= 0) {
    rb_raise(rb_eRangeError, "array must have at least one element");
  }

  return DBL2NUM(calculate_mean(self, array_length));
}

VALUE rb_median(VALUE self) {
  unsigned int array_length;

  Check_Type(self, T_ARRAY);

  array_length = rb_long2int(RARRAY_LEN(self));

  if (array_length <= 0) {
    rb_raise(rb_eRangeError, "array must have at least one element");
  }

  return DBL2NUM(calculate_median(rb_ary_dup(self), array_length));
}

