/*
 * eight.cpp
 *
 *  Created on: Feb 16, 2020
 *      Author: Ha V. Pham
 */
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

string goal = "123804765";
int max_length = 1;

int xgoal[9] ={-1,0,1,2,2,2,1,0,0};
int ygoal[9] ={-1,0,0,0,1,2,2,2,1};
string init = "";

//define node
struct node {
	node(string state) {
		this->state = state;
		this -> dept = 0;
		this->h = 0;
		this->f = 0;
	}
	vector <node*> next;
	vector<string> path;
	string state;
	int dept;
	int h;
	int f;
};

//compare function for sorting vector in greedy
bool compare(node* a, node*b) {
	if (a->h > b->h) {
		return true;
	}
	else if (a->h == b->h && a->path.size() > b->path.size()) {
		return true;
	}
	else {
		return false;
	}
}

//compare function for sorting vector in astar
bool comparef(node* a, node*b) {
	if ((a->f) > (b->f)) {
			return true;
		}
		else if (((a->f) == (b->f)) && a->path.size() > b->path.size()) {
			return true;
		}
		else {
			return false;
		}
}
vector<node*> v; //vector for greedy, astar
vector<string> cv; // list of visited nodes
node* initial = nullptr;
deque <node*> lnodes; //deque for bfs, dfs, ids
deque <node*> ldel;

int findxval(int pos) {
	if (pos == 0 || pos== 1 || pos == 2) {
		return pos;
	}
	else if (pos == 3 || pos== 4 || pos == 5) {
		return pos-3;
	}
	else {
		return pos-6;
	}
}
int findyval(int pos) {
	if (pos == 0 || pos== 1 || pos == 2) {
		return 0;
	}
	else if (pos == 3 || pos== 4 || pos == 5) {
		return 1;
	}
	else {
		return 2;
	}
}
//heur. function for number of tiles out of place
int h1(node* n) {
	int sum = 0;
	for (int i = 1; i<9;i++) {
		int pos1 = n->state.find(to_string(i));
		int pos2 = goal.find(to_string(i));
		if (pos1 != pos2) {
			sum++;
		}
	}
	return sum;
}
//heur. function for manhattan
int h2(node* n) {
	int man = 0;
	for (int i = 1; i < n->state.length() ;i++) {
		int pos = n->state.find(to_string(i));
		int xval = findxval(pos);
		int yval = findyval(pos);
		man = man + abs( xval- xgoal[i]) + abs( yval- ygoal[i]);
	}
	return man;
}
//move space to right
node* move_right(node* n){
	int pos0 = n->state.find('0');
	bool b = ((n->path.size() == 0) || n->path.back() != "left");
	string s = n->state;
	if (((pos0 + 1) != 3) && ((pos0 + 1) != 6) && ((pos0 + 1) != 9) && b) {
		swap(s[pos0],s[pos0+1]);
		vector<string> p = n->path;
		p.push_back("right");
		node* newnode = new node(s);
		newnode->path=p;
		newnode->dept = n->dept + 1;
		return newnode;
	}
	return nullptr;
};
//move space up
node* move_up(node* n){
	int pos0 = n->state.find('0');
	bool b = ((n->path.size() == 0) || n->path.back() != "down");
	string s = n->state;
	if (((pos0 + 1) >3) && b) {
		swap(s[pos0],s[pos0 - 3]);
		vector<string> p = n->path;
		p.push_back("up");
		node* newnode = new node(s);
		newnode->path = p;
		newnode->dept = n->dept + 1;
		return newnode;
	}
	return nullptr;
};

//move space to the left
node* move_left(node* n){
	int pos0 = n->state.find('0');
	bool b = ((n->path.size() == 0) || n->path.back() != "right");
	string s = n->state;
	if (((pos0 + 1) != 1) && ((pos0 + 1) != 4) && ((pos0 + 1) != 7) && b) {
		swap(s[pos0],s[pos0 -1]);
		vector<string> p = n->path;
		p.push_back("left");
		node* newnode = new node(s);
		newnode->path = p;
		newnode->dept = n->dept + 1;
		return newnode;

	}
	return nullptr;
};

// move space down
node* move_down(node* n){
	int pos0 = n->state.find('0');
	bool b = ((n->path.size() == 0) ||n->path.back() != "up");
	string s = n->state;
	if (((pos0 + 1) < 7) && b) {
		swap(s[pos0],s[pos0 + 3]);
		vector<string> p = n->path;
		p.push_back("down");
		node* newnode = new node(s);
		newnode->path = p;
		newnode->dept = n->dept + 1;
		return newnode;
	}
	return nullptr;
}

//expand a node
void expand(node* n, string h = "") {
	node* r = move_right(n);
	node* u = move_up(n);
	node* l = move_left(n);
	node* d = move_down(n);
	if (u != nullptr && u->state != init) {
		if (h == "h1") {
			u->h = h1(u);
			u->f = u->h + u->dept;
		}
		else if (h == "h2") {
			u->h = h2(u);
			u->f = u->h + u->dept;
		}
		n->next.push_back(u);
	}
	if (d != nullptr && d->state != init) {
		if (h == "h1") {
			d->h = h1(d);
			d->f = d->h + d->dept;
		}
		else if (h == "h2") {
			d->h = h2(d);
			d->f = d->h + d->dept;
		}
		n->next.push_back(d);
	}
	if (l != nullptr && l->state != init) {
		if (h == "h1") {
					l->h = h1(l);
					l->f = l->h + l->dept;
				}
				else if (h == "h2") {
					l->h = h2(l);
					l->f = l->h + l->dept;
				}
		n->next.push_back(l);
	}
	if (r != nullptr && r->state != init) {
		if (h == "h1") {
		r->h = h1(r);
		r->f = r->h + r->dept;
		}
		else if (h == "h2") {
			r->h = h2(r);
			r->f = r->h + r->dept;
		}
		n->next.push_back(r);
	}
}

//bfs search function
void bfs(){
	node* n = lnodes.front();
	if (n->state == goal) {
		cout << "(";
		for(int i=0; i < n->path.size() -1; i++) {
			cout << n->path.at(i) << " ";
		}
		if (n->path.size() != 0) {
			cout << n->path.back() << ")" << endl;
		}
	}
	else {
		cv.push_back(n->state);
		lnodes.pop_front();
		expand(n);
		for (int i =0; i < n->next.size(); i++) {
			lnodes.push_back(n->next.at(i));
		}
		if (max_length < lnodes.size()) {
			max_length = lnodes.size();
		}
		bfs();
	}
}

//dfs search fuction
bool dfs(int dept) {
	if (dept == -1) {
	node* n = lnodes.front();
	if (n->state == goal) {
		cout << "(";
		for(int i=0; i < n->path.size() -1; i++) {
			cout << n->path.at(i) << " ";
		}
		if (n->path.size() != 0) {
			cout << n->path.back() << ")" << endl;
		}
	}
	else {
		cv.push_back(n->state);
		lnodes.pop_front();
		expand(n);
		for (int i =0; i < n->next.size(); i++) {
			lnodes.push_front(n->next.at(i));
		}
		if (max_length < lnodes.size()) {
			max_length = lnodes.size();
		}
		dfs(-1);
	}
	return true;
	}
	else {
		node* n = lnodes.front();
		if (n->state == goal) {
			cout << "(";
			for(int i=0; i < n->path.size() -1; i++) {
				cout << n->path.at(i) << " ";
			}
			if (n->path.size() != 0) {
				cout << n->path.back() << ")" << endl;
			}
			return true;
		}
		else {
			cv.push_back(n->state);
			lnodes.pop_front();
			if (n->dept + 1 <= dept) {
				if (n->next.size() == 0) {
				 expand(n);
				}
				for (int i =0; i < n->next.size(); i++) {
					lnodes.push_front(n->next.at(i));
				}
				if (max_length < lnodes.size()) {
					max_length = lnodes.size();
				}
				return dfs(dept);
			}
			else if (lnodes.size() != 0) {
				return dfs(dept);
			}
			else return false;
		}

	}
}

//dls search function
bool dls(node* n, int depth) {
	if (depth == 0) {
		if (n->state == goal) {
			cout << "(";
			for(int i=0; i < n->path.size() -1; i++) {
				cout << n->path.at(i) << " ";
			}
			if (n->path.size() != 0) {
				cout << n->path.back() << ")" << endl;
			}
			return true;
		}
		return false;
	}
	else {
		lnodes.push_front(initial);
		return dfs(depth);
	}
}

//ids search function
void ids() {
	bool stop = false;
	int dept = 0;
	while (!stop) {
		cv.clear();
		stop = dls (initial, dept);
		dept ++;
		lnodes.clear();
	}
}

//greedy search function
void greedy (string h) {
	node* n = v.back();
		if (n->state != goal) {
			bool stop = false;
			expand(n, h);
			for (int i =0; i < n->next.size(); i++) {
				node* temp = n->next.at(i);
				string st = temp -> state;
				for (int i=0; i < cv.size(); i++) {
					if (cv.at(i) == st) {
						stop = true;
						break;
					}
				}
				if (!stop) {
					v.push_back(n->next.at(i));
				}
				if (max_length < v.size()) {
					max_length = v.size();
				}
			}
			sort(v.begin(), v.end(), compare);

			cv.push_back(n->state);
			v.pop_back();
			greedy(h);
		}
		else {
			cout << "(";
			for(int i=0; i < n->path.size() -1; i++) {
				cout << n->path.at(i) << " ";
			}
			if (n->path.size() != 0) {
				cout << n->path.back() << ")" << endl;
			}
		}

}

//astar search function
void astar (string h) {
	node* n = v.back();
		if (n->state != goal) {
			bool stop = false;
			expand(n, h);
			for (int i =0; i < n->next.size(); i++) {
				node* temp = n->next.at(i);
				string st = temp -> state;
				for (int i=0; i < cv.size(); i++) {
					if (cv.at(i) == st) {
						stop = true;
						break;
					}
				}
				if (!stop) {
					v.push_back(n->next.at(i));
				}
				if (max_length < v.size()) {
					max_length = v.size();
				}
			}
			sort(v.begin(), v.end(), comparef);


			cv.push_back(n->state);


			v.pop_back();

			astar(h);
		}
		else {
			cout << "(";
			for(int i=0; i < n->path.size() -1; i++) {
				cout << n->path.at(i) << " ";
			}
			if (n->path.size() != 0) {
				cout << n->path.back() << ")" << endl;
			}
		}

}

int main () {
	double runtime;
	clock_t start = clock();
	char t;
	string h;
	string line, search;
	string st="";
	cout << "Please enter search algorithm and state as form, e.i: (bfs '( 1 2 3 4 5 6 7 8 0)) " << endl;
	getline(cin,line);
	stringstream ss;
	line.erase(0,1);
	line.pop_back();

	if (line.find('h') != std::string::npos) {
		h = line.substr(line.find('h') , 2);
		line.erase(line.find('h'), 2);
	}
	ss << line;
	ss >> search;
	while (!ss.eof()) {
		ss >> t;
		if (isdigit(t)) {
			st = st+t;
		}

	}
	initial = new node(st);
	init = st;

	if (search == "bfs") {
		lnodes.push_front(initial);
		bfs();
		//cout << "max_length = " << max_length << endl;
		//cout << "number of nodes visited = " << cv.size() << endl;
	}
	else if (search == "dfs") {
		lnodes.push_front(initial);
		dfs(-1);
		//cout << "max_length = " << max_length << endl;
		//cout << "number of nodes visited = " << cv.size() << endl;
	}
	else if (search == "ids") {
		lnodes.push_front(initial);
		ids();
		//cout << "max_length = " << max_length << endl;
		//cout << "number of nodes visited = " << cv.size() << endl;
	}
	else if (search == "greedy") {
		//cout << "greedy " << h << endl;
		v.push_back(initial);
		if (h == "h1") {
			initial->h = h1(initial);
		} else {
			initial->h = h2(initial);
		}
		greedy(h);
		//cout << "max_length = " << max_length << endl;
		//cout << "number of nodes visited = " << cv.size() << endl;

	}
	else if (search == "astar") {
		//cout << "astar " << h << endl;
		v.push_back(initial);
		if (h == "h1") {
			initial->h = h1(initial);
		} else {
			initial->h = h2(initial);
		}
		astar(h);
		//cout << "max_length = " << max_length << endl;
		//cout << "number of nodes visited = " << cv.size() << endl;
	}
	//runtime = (double)(clock() - start);
	//cout << "end " << runtime << endl;
	return 1;
}
