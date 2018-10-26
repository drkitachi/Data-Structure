// author: levitan

#ifndef NTREE_H
#define NTREE_H

#include <assert.h>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <string>
#include <vector>
#include <fstream>

namespace std {
	std::string to_string(const std::string& str) { return str; }
}

template <class T>
class NTree {
	struct tnode {
		T data;
		tnode* parent;
		std::vector<tnode*> children;

		tnode (T x) : data(x), parent(NULL) {}

		void addChild(tnode* c) { children.push_back(c); }
	};

	tnode* root;

	// recursive private method called by >> operator
	std::queue<std::string> toStrings(tnode* nd) const {
		std::queue<std::string> rq;
		if (!nd) {	// base case: empty child: return empty vector
			return rq;
		}
		if (nd->children.size()==0) {	// base case: leaf: return vector with just that node's value
			rq.push(std::to_string(nd->data));
			return rq;
		}

		// get queue of strings representing each subtree
		std::vector<std::queue<std::string> > subtrees;
		for (tnode* c : nd->children) {
			subtrees.push_back(toStrings(c));
		}

		// add current value to return queue (will be at front)
		// pad to width of the next level
		int stringWidth = 0;
		int maxDepth = 0;
		for (auto st : subtrees) {
			stringWidth += st.front().length();
			maxDepth = std::max(maxDepth, (int)st.size());
		}
		int levelpad = (stringWidth + subtrees.size() - 1 - std::to_string(nd->data).length())/2;
		rq.push(std::string(levelpad, '_') + std::to_string(nd->data) +
				std::string(levelpad, '_'));

		// add space-padded string(s) to subtrees with fewer levels than max
		for (auto& st : subtrees) {
			int width = st.front().length();
			while (st.size() < maxDepth) {
				st.push(std::string(width, '_'));
			}
		}

		// combine subtrees at each level
		while (!subtrees.front().empty()) {		// all subtrees now have same depth
			std::vector<std::string> levelstrings;	// hold strings at this level
			for (auto& st : subtrees) {
				levelstrings.push_back(st.front());
				st.pop();
			}

			// levelstrings now contains string from each subtree
			// combine into single string and add to rq
			std::string levelstr = std::accumulate(levelstrings.begin(),
					levelstrings.end(), std::string(),
					[](const std::string& a, const std::string& b) -> std::string {
							return a + (a.length() > 0 ? " " : "") + b;
					});
			rq.push(levelstr);

		}	// iterate to next level

		// all subtrees have been processed
		return rq;
	}

	std::string toString(tnode* nd) const {
		std::queue<std::string> q = toStrings(nd);
		std::string str;
		while (!q.empty()) {
			str += (q.front() + "\n");
			q.pop();
		}
		return str;
	}

	//recursive method call by serializeString, builds a queue to be serialized
	//build in a Pre-Order DFS
	void serializeHelper(std::queue<std::string>& q, tnode* nd, int depth){
        if(!nd) return;

        std::string str, str2;

        //depending on the hierarchy, root no tab, one tab for root's child and so on
        for(int i = 0; i < depth; i++)
            str += "\t";

        str += std::to_string(nd -> data);
        q.push(str);

        if(nd->children.empty()){
            for(int i = 0; i < depth; i++)
            str2 += "\t";
            str2 += "-1"; //-1 marks no more child
            q.push(str2);
            return;
        }
        for(auto c: nd -> children)
            serializeHelper(q, c, depth+1); //recursive call to children
        for(int i = 0; i < depth; i++)
            str2 += "\t";
        str2 += "-1";
        q.push(str2);

        return;
    }

    //Builds one big string to output in the file.
    std::string serializeString(){
        std::string str;
        std::queue<std::string> qstr;
	    int depth = 0;

	    serializeHelper(qstr, root, depth);

	    while(!qstr.empty()){
            str += qstr.front() + "\n";
            qstr.pop();
        }

        return str;
    }

    //method to input every element in the file into a Vector
    std::vector<T> buildVector (std::ifstream& inf) {
        std::vector<T> v;
        T item;

        while(inf >> item){
            v.push_back(item);
        }
        return v;
	}

    //Recursive method to deserialize a file and construct a tree
    //Use of Pre-Order DFS
	tnode* deserializeHelper(const std::vector<T>& v, int& index, bool& noAdd) {
        if(index == v.size() || std::to_string(v[index]) == "-1") {
            noAdd = true;
            index += 1;
            return NULL;
        }// marker -1 as base case and no more child

        tnode* nd = new tnode(v[index]);
        index += 1;

        while(index < v.size()){
            tnode* child = deserializeHelper(v,index,noAdd);
            if(noAdd) break; // use flag to drop out if no child
            nd->addChild(child);
        }
        for(tnode* cnd: nd->children) cnd->parent = nd; // link child back to parent

        noAdd = false;

        return nd;
	}

	public:
	NTree() : root(NULL) {}

	NTree(const std::vector<T>& values, const std::vector<int>& parents) {
		if (values.size() != parents.size()) {
			std::cout << "Error: values has " << values.size() << " elements and parents has " << parents.size() << " elements.\n";
			throw std::exception();
		}

		std::map<T, tnode*> valmap;	// will map values to node addresses
		for (int i = 0; i < values.size(); i++) {
			tnode* nd = new tnode(values[i]);
			valmap[values[i]] = nd;
			if (parents[i] >= 0) {		// -1 signals root
				nd->parent = valmap[values[parents[i]]];
				nd->parent->addChild(nd);
			}
			else root = nd;
		}

	}

	~NTree() {
		if (!root) return;
		std::queue<tnode*> q;
		q.push(root);

		while (!q.empty()) {
			tnode* nd = q.front();
			q.pop();
			for (tnode* c : nd->children) q.push(c);
			delete nd;
		}
	}

	friend std::ostream& operator<<(std::ostream& stream, const NTree& tree) {
		stream << tree.toString(tree.root);
		return stream;
	}

	// TODO: replace dummy return value with test for equality
	// USE of BFS to compared every item
	bool operator==(const NTree<T>& rhs) {
		std::queue<tnode*> q1, q2;

        // first call to check for root
		if(!root || !rhs.root) return false;
		q1.push(root); q2.push(rhs.root);

        tnode* c1 = q1.front();
        tnode* c2 = q2.front();

        if(c1->data != c2->data) return false;
        if(c1->children.size() != c2->children.size()) return false;

        q1.pop(); q2.pop();

		while(!(q1.empty() && q2.empty())){
            tnode* c1 = q1.front();
            tnode* c2 = q2.front();

            if(c1->data != c2->data) return false;
            if(c1->parent->data != c2->parent->data) return false;
            if(c1->children.size() != c2->children.size()) return false;

            for(auto d: c1->children) q1.push(d);
            for(auto d: c2->children) q2.push(d);

            q1.pop(); q2.pop();
		}

		return true;
	}


	// TODO: implement method to write tree in recoverable format to file
	void serialize(const std::string& filename) {
	    std::ofstream file;
	    file.open(filename.c_str());
	    if(!file) {
            std::cout << "Error: Could not open " + filename << std::endl;
            throw std::exception();
        }
	    file << serializeString();
	    file.close();
	}

    //For Testing Purpose
    /*
    void printSerializeQueue(){

        std::queue<std::string> q;
        int depth = 0;

        serializeHelper(q, root, depth);

        while(!q.empty()){
            std::cout << q.front() << std::endl;
            q.pop();
        }

    }
    */

	// TODO: implement method to read tree in from file
	void deserialize(const std::string& filename) {
	    std::ifstream file(filename.c_str());
        if(!file) {
            std::cout << "Error: Could not open " + filename << std::endl;
            throw std::exception();
        }
        std::vector<T> v = buildVector(file);
        file.close();

        int i = 0; // index to move in the vector
        bool flag = false;
        root = deserializeHelper(v,i,flag);
    }

    //For testing purpose
    /*
    void printTree(){
        std::queue<tnode*> q;
        q.push(root);

        while (!q.empty()) {
            tnode* cnd = q.front();
            std::cout << cnd->data << std::endl;
            for(auto c: cnd->children)
            q.push(c);
            q.pop();
        }
    }
*/
};

#endif
