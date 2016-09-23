#ifndef Netflix_h
#define Netflix_h

#include <iostream>
#include <string>
#include <utility> //pair()
#include <iomanip> //setprecision()
#include <map>	   //map(), insert(), find(),
//#include <boost/archive/binary_iarchive.hpp>


//#include <boost/serialization/access.hpp>
//#include <boost/serialization/map.hpp>
//#include <cstdint>

//#include "truth_cache.h"

using namespace std;


void netflix_solve (istream& r, 
          ostream& w, 
          map<string,double> m_cache, 
          map<string,double> c_cache, 
          map<string,map<string,double>> a_cache);
//void   make_cache();
//void   make_actual();
bool   netflix_read (istream& r, string& s);
void   netflix_print_movie(ostream& w, string s);
void   netflix_print_rating(ostream& w, double& d);
void   netflix_print_RMSE (ostream& w, double& d);
double netflix_eval (string movie, string customer, map<string,double>& m_cache, map<string,double>& c_cache);
double netflix_RMSE (int c, double error);
double get_actual(string m, string p, map<string, map<string, double> >& a_cache);






/**
 * Class that generates a compact binary representation of the training 
 * ratings which is serializes to disk.
 *
 * This structure allows very easy access to ratings given a customer and
 * movie id.
 */
/*class TruthCache {
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar & this->truth;
  }

  *
   * A lookup table where for each movie id,
   * the value is an associative array mapping 
   * customer ids to their ratings.
   *
   * Note that this is not a 2D array for space reasons, the
   * data is relatively sparse.

  std::map<uint32_t, std::map<uint32_t, uint32_t>> truth;
public:
  TruthCache();
  TruthCache(uint32_t capacity);
  uint32_t get_rating(uint32_t customer_id, uint32_t movie_id);
};*/


#endif // Netflix_h