
#include <string>
#include <iomanip>
#include <utility>
#include <fstream>
#include <cmath>
#include <cassert>
#include <ctime>
//#include <boost/archive/binary_iarchive.hpp>

#include "Netflix.h"


using namespace std;

//map<string,double> m_cache;						//cache for movie and their averaged rating
//map<string,double> c_cache;						//cache for person and their averaged rating
//map<string,map<string,double> > a_cache;		//cache for the actual rating for the customer given a movie

/*TruthCache::TruthCache() : truth() {}

uint32_t TruthCache::get_rating(uint32_t customer_id, uint32_t movie_id) {
      return this->truth[movie_id][customer_id];}*/

//using binary cache
//TruthCache cache;
//using binary cache

void netflix_solve (istream& r, 
					ostream& w, 
					map<string,double> m_cache, 
					map<string,double> c_cache, 
					map<string,map<string,double>> a_cache) {

	//using binary cache
	//ifstream ifs("/u/downing/public_html/netflix-cs371p/az5385-probe-lookup.bin");
	//boost::archive::binary_iarchive ia(ifs);
	//ia >> cache;
	//using binary cache

	//ifstream f;					//file for actaul_user_rating
	string m;						//the movie
	string p;						//the person
	string _p;						//_p is the actaul rating given by p
	double E = 0;					//the total error
	double e;						//the error of individual ratings
	double rating;					//predicted rating of m for p
	int count = 0;					//number of ratings

	//f.open("az6257-actual-user-ratings.txt");
	
	//begin making movie and customer cache
	//make_cache();
	//make_actual();
	//end making movie cache and customer cache

	//while (netflix_read (r, p) && netflix_read(f,_p)) 
	while(netflix_read(r,p))
	{
		//cout << "*" << endl;
		if (p.find(":") != string::npos) 
		{
			m = p;
			netflix_print_movie(w,m);
		}
		else 
		{
			++count;							//increase the count of the number of ratings calculated
			rating = netflix_eval(m,p, m_cache, c_cache);			//calculates a prediced rating
			netflix_print_rating(w, rating);	//prints out the rating in a fixed precision of 1 decimal place
			//e =  stoi(_p);						//gets the actual rating given by customer p
			e = get_actual(m,p, a_cache);
			e -= rating;						//calculates the error
			E += (pow(e,2));					//squares the error and adds it to the total error
			
		}

	}
	//f.close();
	//double d = netflix_RMSE(count, E);
	double d = sqrt(E/((double)count));
	netflix_print_RMSE(w, d);
}
double get_actual(string m, string p, map<string, map<string, double> >& a_cache)
{
	m.pop_back();
	double actual;
	map<string, double>& n_map = a_cache.find(m)->second;
	actual = n_map.find(p)->second;
	return actual;
}
/*{
	ifstream actual;					//used to read actual_user_rating
	ifstream probe;						//used to read probe.txt
	string movie_key;					//will hold movie id as the key
	string key;							//will hold either movie id or customer id
	double value;						//will hold the rating
	string _p;
	actual.open("az6257-actual-user-ratings.txt");
	probe.open("probe.txt");
	while(probe >> key)
	{
		//cout << "*" << endl;
		//assert(false);
		//key.pop_back();
		actual >> _p;
		value = stoi(_p);
		//cout << value << endl;
		//cout << key << " " << value << endl;
		if(key.find(":") != string::npos)		//is the key a movie?
		{
			key.pop_back();
			movie_key = key;
			map<string, double> n_map;
			a_cache.insert(pair<string, map<string,double>>(movie_key,n_map));
			//cout << "movie is " << movie_key << endl;
		}
		else
		{
			//create an entry in map< movie,map< customer, actual rating> >
			//create an entry in map< customer, actual rating >
			//assert(false);
			map<string, double>& n_map = a_cache.find(movie_key)->second;
			n_map.insert(pair<string,double>(key,value));

			//cout << key <<" "<< value<<endl;
		}
	}
	actual.close();
	probe.close();
}

void make_cache()
{
	ifstream movies;					//used to read avg_movies_rating
	ifstream customers; 				//used to read avg_customer_rating
	string movie_key;					//will hold movie id as the key
	string key;							//will hold either movie id or customer id
	double value;						//will hold the rating

	movies.open("cyt276-as63439-avg-movie-ratings.txt");
	while(movies >> key)
	{
		movies >> value;
		//cout << value << endl;
		m_cache.insert(pair<string, double>(key,value));
	}
	movies.close();
	customers.open("cyt276-as63439-avg-customer-ratings.txt");
	while(customers >> key)
	{
		customers >> value;
		c_cache.insert(pair<string,double>(key,value));
	}
	customers.close();
	/*actual.open("az6257-actual-user-ratings.txt");
	probe.open("probe.txt");
	while(probe >> key)
	{
		//cout << "*" << endl;
		//assert(false);
		//key.pop_back();
		actual >> _p;
		value = stoi(_p);
		//cout << value << endl;
		//cout << key << " " << value << endl;
		if(key.find(":") != string::npos)		//is the key a movie?
		{
			key.pop_back();
			movie_key = key;
			map<string, double> n_map;
			a_cache.insert(pair<string, map<string,double>>(movie_key,n_map));
			//cout << "movie is " << movie_key << endl;
		}
		else
		{
			//create an entry in map< movie,map< customer, actual rating> >
			//create an entry in map< customer, actual rating >
			//assert(false);
			map<string, double>& n_map = a_cache.find(movie_key)->second;
			n_map.insert(pair<string,double>(key,value));

			//cout << key <<" "<< value<<endl;
		}
	}
	actual.close();
	probe.close();
}*/

//Reads in next line from input stream r and populates string s with the next line
bool netflix_read (istream& r, string& s) {
	if(!(r >> s))
		return false;			//read fails aka no more lines
	return true;}

//prints out the movie id
void netflix_print_movie (ostream& w, string s) {
	w << s << endl;}

//prints out a rating with one decimal
void netflix_print_rating (ostream& w, double& d) {
	w << fixed << setprecision(1) << d << endl;
}

//prints out the RMSE with two decimals
void netflix_print_RMSE (ostream& w, double& d) {
	//d = floor(100*d)/100;
	w << "RMSE: " << fixed << setprecision(2) << d << endl;
}

//estimates the customer's rating for the movie, returns the estimate
double netflix_eval (string movie, string customer, map<string,double>& m_cache, map<string,double>& c_cache) 
{
	movie.pop_back();
	double m_avg;
	double c_avg;
	double m_offset;
	double c_offset;
	double predict;

	m_avg = m_cache.find(movie)->second;
	c_avg = c_cache.find(customer)->second;

	m_offset = m_avg - 3.7;
	c_offset = c_avg - 3.7;
	predict  = 3.7 + m_offset + c_offset;


	return predict;
}

//takes the count of customers evaluated and the total of the errors squared
//and calculates the Root Mean Square Error
double netflix_RMSE (int c, double error) {

	error = error / (double)c;
	return sqrt(error);
}









