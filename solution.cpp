#include<bits/stdc++.h>
using namespace std;

set<string> included_tx_id;
string inputFile = "mempool.csv";
double maxwt = 4000000.0;

class Transaction
{
public:
    string txid;
    vector<string> parents;
    int fee;
    int weight;
};

//Function to collect the transaction info from the input file row and returning pair of txid and its corresponding Transaction pointer
pair<string, Transaction *> newTransaction(vector<string> &txnInp)
{
    Transaction*  newTxn= new Transaction();
    newTxn->txid = txnInp[0];
    newTxn->fee = stoi(txnInp[1]);
    newTxn->weight = stoi(txnInp[2]);
    vector<string> p;
    for (int i = 3; i < txnInp.size(); i++)
    {
        p.push_back(txnInp[i]);
    }
    newTxn->parents = p;
    return make_pair(txnInp[0], newTxn);
}

void readInput(string inputFile, unordered_map<string, Transaction *> &umap)
{
    ifstream fin(inputFile);
    vector<string> row;         //vector of details of a transaction containing txid,fee, weight, list of parents
    string  line, word;
    getline(fin, line);
    while (getline(fin, line))
    {
        row.clear();
        stringstream s(line);
        while (getline(s, word, ','))
        {
            row.push_back(word);
        }
        pair<string, Transaction *> p = newTransaction(row);
        umap[p.first] = p.second;
    }
    fin.close();
    cout << "Total number of transactions read: " << umap.size() << endl;
}

//Validating the Transaction by checkin that if all of its parent already exists in the block
bool isValidTxn(Transaction *txn, set<string> &set_of_included_txns)
{
    for (auto par : txn->parents)
    {
        if (set_of_included_txns.find(par) == set_of_included_txns.end())
            return false;
    }
    return true;
}
void writeOutput(vector<string> &res_vector, string fn)
{
    ofstream myfile(fn);
    for (auto s : res_vector)
        myfile << s << endl;
    myfile.close();
}
int main()
{
//    Map to track the transactions from their transactions is O(1) time complexity
    unordered_map<string, Transaction *> umap;
    readInput(inputFile, umap);
//    creating a set which will contain the txns in order of their fees/weight ratio
    set<pair<float, Transaction *>, greater<pair<float, Transaction *>>> tx_set;
//    set of transactions successfully added to the block
    set<string> set_of_included_txn;
//    vector of transactions added to maintain to order of the them
    vector<string> res_tx_vector;
    for (auto txnPair : umap)
    {
        tx_set.insert({(float)txnPair.second->fee / (float)txnPair.second->weight, txnPair.second});
    }
    double currBlockWeight = 0.0;
    int totalFee = 0;
    while (!tx_set.empty() && currBlockWeight < maxwt)
    {
        bool found = false;
        for (auto itr = tx_set.begin(); itr != tx_set.end(); itr++)   //finding valid transaction with maximum ratio from transaction set
        {
            Transaction *curr_tx = (*itr).second;
            int currFee = curr_tx->fee;
            int currWeight = curr_tx->weight;
            if (isValidTxn(curr_tx, set_of_included_txn) && (currBlockWeight + currWeight) <= maxwt)
            {
                currBlockWeight += currWeight;
                set_of_included_txn.insert(curr_tx->txid);
                res_tx_vector.push_back(curr_tx->txid);
                totalFee+=currFee;
                tx_set.erase(itr);
                found = true;
                break;
            }
        }
        if (!found)                                 //No valid transactions left
            break;
    }
    cout << "Total Transactions in final block " << set_of_included_txn.size() << "\n";
    cout << "Maximum total fees of the final block : " << totalFee<<" satoshis\n";
    cout << "Total weight of the block: " << currBlockWeight<<"\n";
    cout<<"\n";
    writeOutput(res_tx_vector,"block.txt");
   return 0;
}
