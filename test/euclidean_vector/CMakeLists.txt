cxx_test(
   TARGET euclidean_vector_constructors_assignments_test
   FILENAME "euclidean_vector_constructors_assignments_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET euclidean_vector_friends_utility_test
   FILENAME "euclidean_vector_friends_utility_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET euclidean_vector_ops_methods_test
   FILENAME "euclidean_vector_ops_methods_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)