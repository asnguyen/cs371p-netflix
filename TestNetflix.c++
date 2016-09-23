// --------------------------------
// projects/collatz/TestCollatz.c++
// Copyright (C) 2016
// Glenn P. Downing
// --------------------------------

// https://code.google.com/p/googletest/wiki/V1_7_Primer#Basic_Assertions

// --------
// includes
// --------

#include <iostream> // cout, endl
#include <sstream>  // istringtstream, ostringstream
#include <string>   // string
#include <map>
#include <utility>


#include "gtest/gtest.h"
#include "Netflix.h"

using namespace std;

//netflix_read test begin
TEST(NetflixFixture, read_1)
{
	istringstream r("1234\n");
	string s;
	const bool b = netflix_read(r,s);
	ASSERT_TRUE(b);
	ASSERT_EQ("1234",s);
}
TEST(NetflixFixture, read_2)
{
	istringstream r("1234:\n");
	string s;
	const bool b = netflix_read(r,s);
	ASSERT_TRUE(b);
	ASSERT_EQ("1234:",s);
}
TEST(NetflixFixture, read_3)
{
	istringstream r("");
	string s;
	const bool b = netflix_read(r,s);
	ASSERT_FALSE(b);
}
//netflix_read test end

//netflix_print_movie test begin
TEST(NetflixFixture, print_movie_1)
{
	ostringstream w;
	netflix_print_movie(w, "1234:");
	ASSERT_EQ("1234:\n",w.str());
}
TEST(NetflixFixture, print_movie_2)
{
	ostringstream w;
	netflix_print_movie(w, "1:");
	ASSERT_EQ("1:\n",w.str());
}
TEST(NetflixFixture, print_movie_3)
{
	ostringstream w;
	netflix_print_movie(w, "10:");
	ASSERT_EQ("10:\n",w.str());
}
//netflix_print_movie test end


//netflix_print_rating test begin
TEST(NetflixFixture, print_rating_1)
{
	ostringstream w;
	double d = 1.2;
	netflix_print_rating(w,d);
	ASSERT_EQ("1.2\n",w.str());
}
TEST(NetflixFixture, print_rating_2)
{
	ostringstream w;
	double d = 3.8123456;
	netflix_print_rating(w,d);
	ASSERT_EQ("3.8\n",w.str());
}
TEST(NetflixFixture, print_rating_3)
{
	ostringstream w;
	double d = 4;
	netflix_print_rating(w,d);
	ASSERT_EQ("4.0\n",w.str());
}
//netflix_print_rating test end

//netflix_print_RMSE test begin
TEST(NetflixFixture, print_RMSE_1)
{
	ostringstream w;
	double d = .95;
	netflix_print_RMSE(w,d);
	ASSERT_EQ("RMSE: 0.95\n",w.str());
}
TEST(NetflixFixture, print_RMSE_2)
{
	ostringstream w;
	double d = 1;
	netflix_print_RMSE(w,d);
	ASSERT_EQ("RMSE: 1.00\n",w.str());
}
TEST(NetflixFixture, print_RMSE_3)
{
	ostringstream w;
	double d = .987654321;
	netflix_print_RMSE(w,d);
	ASSERT_EQ("RMSE: 0.99\n",w.str());
}
//netflix_print_RSME test end


//netflix_eval test begin
TEST(NetflixFixture, eval_1)
{
	//make_cache();
	string movie    = "1:";
	string customer = "30878";
	map<string, double> m;
	m.insert(pair<string,double>("1",3.7));
	map<string, double> c;
	c.insert(pair<string,double>("30878",3.7));
	ostringstream w;
	const double predict  = netflix_eval(movie,customer,m,c);
	w << fixed << setprecision(1) << predict << endl;
	ASSERT_EQ("3.7\n",w.str());
}

TEST(NetflixFixture, eval_2)
{
	//make_cache();
	string movie    = "2:";
	string customer = "1234";
	map<string, double> m;
	m.insert(pair<string,double>("2",4.2));
	map<string, double> c;
	c.insert(pair<string,double>("1234",2.1));
	ostringstream w;
	const double predict  = netflix_eval(movie,customer,m,c);
	w << fixed << setprecision(1) << predict << endl;
	ASSERT_EQ("2.6\n",w.str());
}
TEST(NetflixFixture, eval_3)
{
	//make_cache();
	string movie    = "3:";
	string customer = "5678";
	map<string, double> m;
	m.insert(pair<string,double>("3",4.1));
	map<string, double> c;
	c.insert(pair<string,double>("5678",2.9));
	ostringstream w;
	const double predict  = netflix_eval(movie,customer,m,c);
	w << fixed << setprecision(1) << predict << endl;
	ASSERT_EQ("3.3\n",w.str());
}
//netflix_eval test end


//get_actual test begin
TEST(NetflixFixturem, get_actual_1)
{
	string movie = "1:";
	string customer = "30878";
	map<string,double> n;
	n.insert(pair<string,double>("30878",4));
	map<string, map<string, double>> a;
	a.insert(pair<string,map<string,double>>("1",n));
	const double rating = get_actual(movie,customer,a);
	ASSERT_EQ(4,rating);
}
TEST(NetflixFixturem, get_actual_2)
{
	string movie = "2:";
	string customer = "1234";
	map<string,double> n;
	n.insert(pair<string,double>("1234",3));
	map<string, map<string, double>> a;
	a.insert(pair<string,map<string,double>>("2",n));
	const double rating = get_actual(movie,customer,a);
	ASSERT_EQ(3,rating);
}
TEST(NetflixFixturem, get_actual_3)
{
	string movie = "3:";
	string customer = "5678";
	map<string,double> n;
	n.insert(pair<string,double>("5678",1));
	map<string, map<string, double>> a;
	a.insert(pair<string,map<string,double>>("3",n));
	const double rating = get_actual(movie,customer,a);
	ASSERT_EQ(1,rating);
}
//get_actual test end

//netflix_solve test begin
TEST(NetflixFixture, solve_1)
{
	istringstream r("10009:\n1\n");
	ostringstream w;
	map<string, double> m;
	map<string, double> c;
	map<string,double> n;
	map<string, map<string, double>> a;

	m.insert(pair<string,double>("10009",4.1));
	c.insert(pair<string,double>("1",2.9));
	n.insert(pair<string,double>("1",3));
	a.insert(pair<string,map<string,double>>("10009",n));


	netflix_solve(r,w,m,c,a);
	ASSERT_EQ("10009:\n3.3\nRMSE: 0.30\n",w.str());

}

TEST(NetflixFixture, solve_2)
{
	istringstream r("14357:\n1079\n");
	ostringstream w;
	map<string, double> m;
	map<string, double> c;
	map<string,double> n;
	map<string, map<string, double>> a;

	m.insert(pair<string,double>("14357",3.7));
	c.insert(pair<string,double>("1079",3.7));
	n.insert(pair<string,double>("1079",3));
	a.insert(pair<string,map<string,double>>("14357",n));


	netflix_solve(r,w,m,c,a);
	ASSERT_EQ("14357:\n3.7\nRMSE: 0.70\n",w.str());
}

TEST(NetflixFixture, solve_3)
{
	
	istringstream r("7633:\n1257\n");
	ostringstream w;
	map<string, double> m;
	map<string, double> c;
	map<string,double> n;
	map<string, map<string, double>> a;

	m.insert(pair<string,double>("7633",3.8));
	c.insert(pair<string,double>("1257",3.8));
	n.insert(pair<string,double>("1257",3));
	a.insert(pair<string,map<string,double>>("7633",n));


	netflix_solve(r,w,m,c,a);
	ASSERT_EQ("7633:\n3.9\nRMSE: 0.90\n",w.str());
}

//netflix_solve test end
































