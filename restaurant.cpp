#include "main.h"

int MAXSIZE;

struct TreeNode {
    TreeNode *pLeft;
    TreeNode *pRight;
    int wgt;
    char letter;
    bool isLeaf;
    string code;

    TreeNode() {
        pLeft = nullptr;
        pRight = nullptr;
        wgt = 0;
        isLeaf = false;
    }
};

struct ListNode {
    char alphabet;
    int frequency;
    TreeNode *inTree;
    bool isLetter;
};

struct BSTNode {
    BSTNode *pLeft;
    BSTNode *pRight;
    int key;
};

struct HeapNode {
    int area;
    int value;
};

struct SukunaCus {
    int ID;
    int Result;
};

void deleteHuffTree(TreeNode *root) {
    if (!root) {
        return;
    }
    deleteHuffTree(root->pLeft);
    deleteHuffTree(root->pRight);
    delete root;
}

void deleteBST(BSTNode *root) {
    if (!root) {
        return;
    }
    deleteBST(root->pLeft);
    deleteBST(root->pRight);
    delete root;
}

class Restaurant {
public:
    TreeNode *newestCus;

    vector<vector<BSTNode *>> hashGoJo;

    vector<vector<SukunaCus>> SukunaRes;
    vector<HeapNode> minHeap;
    vector<int> LRU;
    int recent;

    Restaurant() {
        newestCus = nullptr;
        recent = -2e9;
    };

    ~Restaurant() {
        deleteHuffTree(newestCus);
        if (!hashGoJo.empty()) {
            for (int i = 1; i <= MAXSIZE; i++) {
                deleteBST(hashGoJo[0][i]);
            }
        }
    };

    void LAPSE(const string &name);

    void KOKUSEN();

    void KEITEIKEN(int num);

    void HAND() const;

    void LIMITLESS(int num);

    void CLEAVE(int num) const;
};

void simulate(string filename) {
    Restaurant *r = new Restaurant();
    ifstream ss(filename);
    string str, maxsize, name, num;
    while (ss >> str) {
        if (str == "MAXSIZE") {
            ss >> maxsize;
            MAXSIZE = stoi(maxsize);
        } else if (str == "LAPSE") {
            ss >> name;
            r->LAPSE(name);
        } else if (str == "KOKUSEN") {
            r->KOKUSEN();
        } else if (str == "KEITEIKEN") {
            ss >> num;
            r->KEITEIKEN(stoi(num));
        } else if (str == "HAND") {
            r->HAND();
        } else if (str == "LIMITLESS") {
            ss >> num;
            r->LIMITLESS(stoi(num));
        } else if (str == "CLEAVE") {
            ss >> num;
            r->CLEAVE(stoi(num));
        }
    }
    delete r;
}

TreeNode *rotateRight(TreeNode *root) {
    TreeNode *l = root->pLeft;
    TreeNode *lr = l->pRight;
    l->pRight = root;
    root->pLeft = lr;
    return l;
}

TreeNode *rotateLeft(TreeNode *root) {
    TreeNode *r = root->pRight;
    TreeNode *rl = r->pLeft;
    r->pLeft = root;
    root->pRight = rl;
    return r;
}

int getHeight(TreeNode *node) {
    if (node == nullptr) {
        return 0;
    }
    int lh = getHeight(node->pLeft);
    int rh = getHeight(node->pRight);
    return (lh > rh ? lh : rh) + 1;
}

int getBalance(TreeNode *t) {
    if (!t) {
        return 0;
    }
    return getHeight(t->pLeft) - getHeight(t->pRight);
}

bool less3diff(const string &name) {
    vector<int> al(52, 0);
    int numChar = 0;
    for (char c: name) {
        if (c >= 'a' && c <= 'z') {
            if (al[int(c - 'a')] == 0) {
                numChar++;
            }
            al[int(c - 'a')]++;
        } else if (c >= 'A' && c <= 'Z') {
            if (al[int(c - 'A') + 26] == 0) {
                numChar++;
            }
            al[int(c - 'A') + 26]++;
        }
        if (numChar >= 3) {
            return false;
        }
    }
    return true;
}

vector<char> nameCaesar(const string &name) {
    vector<char> caesar;
    vector<int> freq(52, 0);
    for (char c: name) {
        if (c >= 'a' && c <= 'z') {
            freq[int(c - 'a')]++;
        } else if (c >= 'A' && c <= 'Z') {
            freq[int(c - 'A') + 26]++;
        }
    }
    for (char c: name) {
        if (c >= 'a' && c <= 'z') {
            int k = int(c) + (freq[int(c - 'a')]) % 26;
            while (k > int('z')) {
                k -= 26;
            }
            caesar.push_back(char(k));
        } else if (c >= 'A' && c <= 'Z') {
            int k = int(c) + (freq[int(c - 'A' + 26)]) % 26;
            while (k > int('Z')) {
                k -= 26;
            }
            caesar.push_back(char(k));
        }
    }
    return caesar;
}

vector<ListNode> buildListX(const string &name) {
    vector<int> alpha(52, 0);
    for (char c: name) {
        if (c >= 'a' && c <= 'z') {
            alpha[int(c - 'a')]++;
        } else if (c >= 'A' && c <= 'Z') {
            alpha[int(c - 'A') + 26]++;
        }
    }
    vector<int> beta(52, 0);
    for (int i = 0; i < 26; i++) {
        if (alpha[i] > 0) {
            beta[(i + alpha[i]) % 26] += alpha[i];
        }
    }
    for (int i = 26; i < 52; i++) {
        if (alpha[i] > 0) {
            beta[(i - 26 + alpha[i]) % 26 + 26] += alpha[i];
        }
    }
    vector<ListNode> listX;
    for (int i = 0; i < 26; i++) {
        if (beta[i] > 0) {
            ListNode node{};
            node.isLetter = true;
            node.frequency = beta[i];
            node.alphabet = char('a' + i);
            listX.push_back(node);
        }
    }
    for (int i = 26; i < 52; i++) {
        if (beta[i] > 0) {
            ListNode node{};
            node.isLetter = true;
            node.frequency = beta[i];
            node.alphabet = char('A' + (i - 26));
            listX.push_back(node);
        }
    }
    int sz = int(listX.size());
    for (int i = 1; i < sz; i++) {
        int j = i - 1;
        ListNode rem = listX[i];
        while (j >= 0 && listX[j].frequency > rem.frequency) {
            listX[j + 1] = listX[j];
            j--;
        }
        listX[j + 1] = rem;
    }
    return listX;
}

TreeNode *AVL(TreeNode *t, int &numRot) {
    if (numRot == 1 || t == nullptr) {
        return t;
    }
    if (getBalance(t) > 1 && getBalance(t->pLeft) < 0 && numRot < 1) {
        t->pLeft = rotateLeft(t->pLeft);
        t = rotateRight(t);
        numRot++;
    } else if (getBalance(t) > 1 && getBalance(t->pLeft) >= 0 && numRot < 1) {
        t = rotateRight(t);
        numRot++;
    } else if (getBalance(t) < -1 && getBalance(t->pRight) > 0 && numRot < 1) {
        t->pRight = rotateRight(t->pRight);
        t = rotateLeft(t);
        numRot++;
    } else if (getBalance(t) < -1 && getBalance(t->pRight) <= 0 && numRot < 1) {
        t = rotateLeft(t);
        numRot++;
    }
    if (numRot < 1) {
        t->pLeft = AVL(t->pLeft, numRot);
    }
    if (numRot < 1) {
        t->pRight = AVL(t->pRight, numRot);
    }
    return t;
}

TreeNode *buildRootHuff(vector<ListNode> &listX) {
    TreeNode *rootHuff = nullptr;
    if (listX.size() == 1) {
        rootHuff = new TreeNode;
        rootHuff->isLeaf = true;
        rootHuff->pLeft = nullptr;
        rootHuff->pRight = nullptr;
        rootHuff->wgt = listX[0].frequency;
        rootHuff->letter = listX[0].alphabet;
        listX[0].inTree = rootHuff;
    } else if (listX.size() > 1) {
        while (listX.size() > 1) {
            ListNode left = listX[0];
            ListNode right = listX[1];
            listX.erase(listX.begin());
            listX.erase(listX.begin());
            TreeNode *l;
            if (left.isLetter) {
                l = new TreeNode;
                l->isLeaf = true;
                l->pLeft = nullptr;
                l->pRight = nullptr;
                l->wgt = left.frequency;
                l->letter = left.alphabet;
            } else {
                l = left.inTree;
            }
            TreeNode *r;
            if (right.isLetter) {
                r = new TreeNode;
                r->isLeaf = true;
                r->pLeft = nullptr;
                r->pRight = nullptr;
                r->wgt = right.frequency;
                r->letter = right.alphabet;
            } else {
                r = right.inTree;
            }
            TreeNode *mid = new TreeNode;
            mid->isLeaf = false;
            mid->pLeft = l;
            mid->pRight = r;
            mid->wgt = l->wgt + r->wgt;
            for (int i = 0; i < 3; i++) {
                int numRot = 0;
                mid = AVL(mid, numRot);
                if (numRot < 1) {
                    break;
                }
            }
            if (mid->isLeaf) {
                for (auto &a: listX) {
                    deleteHuffTree(a.inTree);
                }
                deleteHuffTree(mid);
                return nullptr;
            }
            ListNode newNode{};
            newNode.inTree = mid;
            newNode.isLetter = false;
            newNode.frequency = mid->wgt;
            int whereAdd = 0;
            while (whereAdd < int(listX.size()) && listX[whereAdd].frequency <= newNode.frequency) {
                whereAdd++;
            }
            listX.insert(listX.begin() + whereAdd, newNode);
        }
        rootHuff = listX[0].inTree;
    }
    return rootHuff;
}

vector<string> getCodeHuff(TreeNode *root) {
    vector<string> codeHuff(52, "");
    queue<TreeNode *> q;
    if (root->isLeaf && !root->pLeft && !root->pLeft) {
        char c = root->letter;
        if (c >= 'a' && c <= 'z') {
            codeHuff[c - 'a'] = "0";
        } else if (c >= 'A' && c <= 'Z') {
            codeHuff[c - 'A' + 26] = "0";
        }
        return codeHuff;
    }
    root->code = "";
    q.push(root);
    while (!q.empty()) {
        TreeNode *t = q.front();
        q.pop();
        if (t->isLeaf) {
            char c = t->letter;
            if (c >= 'a' && c <= 'z') {
                codeHuff[c - 'a'] = t->code;
            } else if (c >= 'A' && c <= 'Z') {
                codeHuff[c - 'A' + 26] = t->code;
            }
        }
        if (t->pLeft) {
            t->pLeft->code = t->code + "0";
            q.push(t->pLeft);
        }
        if (t->pRight) {
            t->pRight->code = t->code + "1";
            q.push(t->pRight);
        }
    }
    return codeHuff;
}

string getNameCode(vector<string> codeHuff, const vector<char> &caesar) {
    string nameByCode;
    for (char c: caesar) {
        if (c >= 'a' && c <= 'z') {
            nameByCode += codeHuff[c - 'a'];
        } else if (c >= 'A' && c <= 'Z') {
            nameByCode += codeHuff[c - 'A' + 26];
        }
    }
    return nameByCode;
}

int getResult(string nameByCode) {
    int len = int(nameByCode.size());
    int num = 0;
    for (int i = len - 1; i >= 0; i--) {
        num = num * 2 + (nameByCode[i] - '0');
        if (i == len - 10) {
            break;
        }
    }
    return num;
}

BSTNode *addBST(BSTNode *pNew, BSTNode *t) {
    if (t == nullptr) {
        t = pNew;
    } else if (pNew->key < t->key) {
        t->pLeft = addBST(pNew, t->pLeft);
    } else {
        t->pRight = addBST(pNew, t->pRight);
    }
    return t;
}

void reheapDown(vector<HeapNode> &minHeap, int len, int index, vector<int> LRU) {
    if (index >= len) {
        return;
    }
    int mini = index;
    int l = index * 2 + 1;
    int r = index * 2 + 2;
    if (l < len && (minHeap[l].value < minHeap[mini].value ||
                    (minHeap[l].value == minHeap[mini].value && LRU[minHeap[l].area] < LRU[minHeap[mini].area]))) {
        mini = l;
    }
    if (r < len && (minHeap[r].value < minHeap[mini].value ||
                    (minHeap[r].value == minHeap[mini].value && LRU[minHeap[r].area] < LRU[minHeap[mini].area]))) {
        mini = r;
    }
    if (mini != index) {
        HeapNode tmp = minHeap[mini];
        minHeap[mini] = minHeap[index];
        minHeap[index] = tmp;
        reheapDown(minHeap, len, mini, LRU);
    }
}

void reheapUp(vector<HeapNode> &minHeap, int len, int index, vector<int> LRU) {
    if (index >= len) {
        return;
    }
    int parent = (index - 1) / 2;
    while (index > 0 && (minHeap[parent].value > minHeap[index].value ||
                         (minHeap[parent].value == minHeap[index].value &&
                          LRU[minHeap[parent].area] > LRU[minHeap[index].area]))) {
        HeapNode tmp = minHeap[parent];
        minHeap[parent] = minHeap[index];
        minHeap[index] = tmp;
        index = parent;
        parent = (index - 1) / 2;
    }
}

void Restaurant::LAPSE(const string &name) {
    if (less3diff(name)) {
        return;
    }
    vector<char> caesar = nameCaesar(name);
    vector<ListNode> listX = buildListX(name);
    if (listX.empty()) {
        return;
    }
    TreeNode *rootHuff = buildRootHuff(listX);
    if (rootHuff == nullptr) {
        return;
    }
    if (newestCus != nullptr) {
        deleteHuffTree(newestCus);
    }
    newestCus = rootHuff;
    vector<string> codeHuff = getCodeHuff(rootHuff);
    string nameByCode = getNameCode(codeHuff, caesar);
    int Result = getResult(nameByCode);
    int ID = Result % MAXSIZE + 1;
    if (Result % 2 == 1) {
        if (hashGoJo.empty()) {
            vector<BSTNode *> emp;
            hashGoJo.resize(MAXSIZE + 1, emp);
            vector<BSTNode *> areaRoots(MAXSIZE + 1, nullptr);
            hashGoJo[0] = areaRoots;
        }
        BSTNode *node = new BSTNode;
        node->pLeft = nullptr;
        node->pRight = nullptr;
        node->key = Result;
        if (!hashGoJo[ID].empty()) {
            addBST(node, hashGoJo[0][ID]);
        } else {
            hashGoJo[0][ID] = node;
        }
        hashGoJo[ID].push_back(node);
    } else {
        SukunaCus newCus{};
        newCus.ID = ID;
        newCus.Result = Result;
        if (SukunaRes.empty()) {
            vector<SukunaCus> emp;
            SukunaRes.resize(MAXSIZE + 1, emp);
            LRU.resize(MAXSIZE + 1, -2e9);
            recent = -2e9;
        }
        LRU[ID] = ++recent;
        SukunaRes[ID].push_back(newCus);
        bool notHad = true;
        int pos;
        for (int i = 0; i < int(minHeap.size()); i++) {
            if (minHeap[i].area == ID) {
                minHeap[i].value++;
                pos = i;
                notHad = false;
                break;
            }
        }
        if (notHad) {
            HeapNode nodeHeap{};
            nodeHeap.area = ID;
            nodeHeap.value = 1;
            minHeap.push_back(nodeHeap);
            pos = int(minHeap.size()) - 1;
        }
        if (pos > 0 && (minHeap[pos].value < minHeap[(pos - 1) / 2].value ||
                        (minHeap[pos].value == minHeap[(pos - 1) / 2].value &&
                         LRU[minHeap[pos].area] < LRU[minHeap[(pos - 1) / 2].area]))) {
            reheapUp(minHeap, int(minHeap.size()), pos, LRU);
        } else {
            reheapDown(minHeap, int(minHeap.size()), pos, LRU);
        }
    }
}

long long combination(int l, int r) {
    if (r == 0 || l == 0) {
        return 1;
    }
    long long comb = 1;
    for (int i = 1; i <= l; i++) {
        comb = (comb * (r + i) / i);
    }
    return comb;
}

int countBST(BSTNode *t) {
    if (!t) {
        return 0;
    }
    int count = 0;
    queue<BSTNode *> q;
    q.push(t);
    while (!q.empty()) {
        BSTNode *n = q.front();
        q.pop();
        count++;
        if (n->pLeft) {
            q.push(n->pLeft);
        }
        if (n->pRight) {
            q.push(n->pRight);
        }
    }
    return count;
}

long long permutation(BSTNode *t) {
    if (!t) {
        return 1;
    }
    long long permu = combination(countBST(t->pLeft), countBST(t->pRight));
    if (t->pLeft) {
        permu = permu * permutation(t->pLeft);
    }
    if (t->pRight) {
        permu = permu * permutation(t->pRight);
    }
    return permu;
}

BSTNode *removeBSTNode(BSTNode *t, BSTNode *node, vector<vector<BSTNode *>> &hashGoJo) {
    if (t == nullptr) {
        return t;
    } else if (t == node) {
        BSTNode *tmp = t;
        if (!t->pLeft) {
            int area = t->key % MAXSIZE + 1;
            if (t == hashGoJo[0][area]) {
                hashGoJo[0][area] = t->pRight;
            }
            t = t->pRight;
            delete tmp;
        } else if (!t->pRight) {
            int area = t->key % MAXSIZE + 1;
            if (t == hashGoJo[0][area]) {
                hashGoJo[0][area] = t->pLeft;
            }
            t = t->pLeft;
            delete tmp;
        } else {
            BSTNode *m = t->pRight;
            while (m->pLeft) {
                m = m->pLeft;
            }
            t->key = m->key;
            int area = m->key % MAXSIZE + 1;
            for (BSTNode *&a: hashGoJo[area]) {
                if (a == m) {
                    a = t;
                    break;
                }
            }
            t->pRight = removeBSTNode(t->pRight, m, hashGoJo);
        }
    } else if (node->key < t->key) {
        t->pLeft = removeBSTNode(t->pLeft, node, hashGoJo);
    } else {
        t->pRight = removeBSTNode(t->pRight, node, hashGoJo);
    }
    return t;
}

void Restaurant::KOKUSEN() {
    if (int(hashGoJo.size()) != MAXSIZE + 1) {
        return;
    }
    for (int i = 1; i <= MAXSIZE; i++) {
        long long Y = permutation(hashGoJo[0][i]);
        Y = Y % MAXSIZE;
        if (Y > int(hashGoJo[i].size())) {
            Y = int(hashGoJo[i].size());
        }
        for (int j = 0; j < Y; j++) {
            hashGoJo[0][i] = removeBSTNode(hashGoJo[0][i], hashGoJo[i][0], hashGoJo);
            hashGoJo[i].erase(hashGoJo[i].begin());
        }
    }
}

vector<int> kickList(vector<int> LRU, vector<vector<SukunaCus>> SukunaRes) {
    vector<int> order;
    if (SukunaRes.empty()) {
        return order;
    }
    for (int i = 1; i <= MAXSIZE; i++) {
        if (!SukunaRes[i].empty()) {
            order.push_back(i);
        }
    }
    int n = int(order.size());
    for (int i = 1; i < n; i++) {
        for (int j = i; j > 0 && (SukunaRes[order[j]].size() < SukunaRes[order[j - 1]].size() ||
                                  (SukunaRes[order[j]].size() == SukunaRes[order[j - 1]].size() &&
                                   LRU[order[j]] < LRU[order[j - 1]])); j--) {
            int tmp = order[j];
            order[j] = order[j - 1];
            order[j - 1] = tmp;
        }
    }
    return order;
}

void removeHeap(vector<HeapNode> &minHeap, int pos, vector<int> LRU) {
    int heapSize = int(minHeap.size());
    minHeap[pos] = minHeap[heapSize - 1];
    minHeap.pop_back();
    if (pos > 0 && (minHeap[pos].value < minHeap[(pos - 1) / 2].value ||
                    (minHeap[pos].value == minHeap[(pos - 1) / 2].value &&
                     LRU[minHeap[pos].area] < LRU[minHeap[(pos - 1) / 2].area]))) {
        reheapUp(minHeap, int(minHeap.size()), pos, LRU);
    } else {
        reheapDown(minHeap, int(minHeap.size()), pos, LRU);
    }
}

void Restaurant::KEITEIKEN(int num) {
    vector<int> kickOrder = kickList(LRU, SukunaRes);
    if (kickOrder.empty()) {
        return;
    }
    vector<SukunaCus> kicked;
    for (int i = 0; i < num; i++) {
        if (i >= int(kickOrder.size())) {
            break;
        }
        int area = kickOrder[i];
        int numKicked = num;
        for (int j = 0; j < num; j++) {
            if (SukunaRes[area].empty()) {
                numKicked = j;
                break;
            }
            kicked.push_back(SukunaRes[area][0]);
            SukunaRes[area].erase(SukunaRes[area].begin());
        }
        int heapSize = int(minHeap.size());
        int pos;
        for (int k = 0; k < heapSize; k++) {
            if (minHeap[k].area == area) {
                pos = k;
                break;
            }
        }
        minHeap[pos].value = minHeap[pos].value - numKicked;
        LRU[area] = ++recent;
        if (minHeap[pos].value <= 0) {
            removeHeap(minHeap, pos, LRU);
        } else if (minHeap[pos].value > 0) {
            reheapUp(minHeap, int(minHeap.size()), pos, LRU);
        }
    }
    for (SukunaCus cus: kicked) {
        cout << cus.Result << "-" << cus.ID << endl;
    }
}

void printInOrderHuff(TreeNode *t) {
    if (!t) {
        return;
    }
    if (t->pLeft) {
        printInOrderHuff(t->pLeft);
    }
    if (t->isLeaf) {
        cout << t->letter << endl;
    } else {
        cout << t->wgt << endl;
    }
    if (t->pRight) {
        printInOrderHuff(t->pRight);
    }
}

void Restaurant::HAND() const {
    printInOrderHuff(newestCus);
}

void printInOrderBST(BSTNode *t) {
    if (!t) {
        return;
    }
    if (t->pLeft) {
        printInOrderBST(t->pLeft);
    }
    cout << t->key << endl;
    if (t->pRight) {
        printInOrderBST(t->pRight);
    }
}

void Restaurant::LIMITLESS(int num) {
    if (num > MAXSIZE || num < 1) {
        return;
    }
    if (num < int(hashGoJo.size())) {
        if (!hashGoJo[num].empty()) {
            printInOrderBST(hashGoJo[0][num]);
        }
    }
}

void printHeapPre(vector<HeapNode> minHeap, int pos, vector<vector<SukunaCus>> SukunaRes, int num) {
    if (pos >= int(minHeap.size())) {
        return;
    }
    int k = 0;
    for (int i = int(SukunaRes[minHeap[pos].area].size()) - 1; i >= 0; i--) {
        cout << SukunaRes[minHeap[pos].area][i].ID << "-" << SukunaRes[minHeap[pos].area][i].Result << endl;
        if (++k == num) {
            break;
        }
    }
    printHeapPre(minHeap, pos * 2 + 1, SukunaRes, num);
    printHeapPre(minHeap, pos * 2 + 2, SukunaRes, num);
}

void Restaurant::CLEAVE(int num) const {
    printHeapPre(minHeap, 0, SukunaRes, num);
}