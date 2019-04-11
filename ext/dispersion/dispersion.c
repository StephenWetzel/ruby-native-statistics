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

double calculate_mean(VALUE array, unsigned long array_length){
  unsigned long i;
  double total = 0;
  double mean = 0;

  for(i = 0; i < array_length; i++){
    total += rb_num2dbl(rb_ary_entry(array, i));
  }

  mean = total / array_length;

  return mean;
}

double calculate_median(VALUE array, unsigned long array_length){
  int n;
  sort_array(array);
  n = (array_length + 1) / 2 - 1
  return array[n]
}

void swap(int *p, int *q) {
   int t;
   t = *p;
   *p = *q;
   *q = t;
}

VALUE sort_array(VALUE array, unsigned long array_length){
   int ii, jj, temp;

   for(ii = 0; ii < n-1; ii++) {
      for(jj = 0; jj < n-ii-1; jj++) {
         if(a[jj] > a[jj+1])
            swap(&a[jj], &a[jj+1]);
      }
   }
}

double calculate_total_distance_from_mean(VALUE array, unsigned long array_length){
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

  return DBL2NUM(calculate_median(self, array_length));
}

