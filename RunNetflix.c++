#include <iostream>
#include <ctime>
#include <utility>
#include <map>
#include <fstream>
#include <string>

#include "Netflix.h"

void make_actual();
void make_cache();

using namespace std;

map<string,double> m_cache;
map<string,double> c_cache;
map<string,map<string,double> > a_cache;

int main () 
{
	
	//cout << "start making actual cache" << endl;
	make_actual();
	//cout << "done making actual cache" << endl;
	//cout << "start making cache" << endl;
	make_cache();
	//cout << "done making cache" << endl;
	//cout << "start netflix_solve" << endl;
	netflix_solve(cin, cout, m_cache, c_cache, a_cache);
	//cout << "done netflix_solve solve" << endl;
	return 0;
}

void make_actual()
{
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
	probe.close();*/
}




