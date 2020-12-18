
#include <vector>
#include <string>
#include <bitset>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <random>
#include <chrono>

#include <fstream>
#include <bitcoin/bitcoin.hpp>

using std::ofstream;
using std::cout;
using std::vector;
using std::string;

using std::string;
using std::to_string;
using std::bitset;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::stringstream;

struct User{
    string name;
    string public_key;
    double balance;
};
struct Transaction{
    string transactionID;
    string senderKey;
    string recipientKey;
    double sum;
    bool completed;


};
struct Block{
    string prevBlockHash;
    time_t timeStamp;
    string version;
    string merkelRootHash;
    uint32_t nonce;
    uint32_t difficultyTarget;
    vector<Transaction> Transactions;
};

string hash_function(string fraze);
Block SearchForNextBlock(const Block& lastBlock, const vector<vector<Transaction>>& t, int& chosenTransaction);
string getBlockMerkelRootHash(const vector<Transaction>& t);
vector<string> getVectorMerkelRootHash(const vector<string>& t);
bool transactionConfirmation(Transaction takenTransaction, vector<User>& AllUsers);
void completeTransaction(Transaction transactionPool,vector<User>& users);
bc::hash_digest create_merkle(bc::hash_list& merkle);

string popfunction(string hex);
string xorfunction(string a,string b);
string andfunction(string a, string b);
string notfunction(string a);
string orfunction(string a, string b);

int main(){
    std::random_device rd;
    std::mt19937::result_type seed = rd() ^ ((std::mt19937::result_type)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() +(std::mt19937::result_type)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() );
    std::mt19937 gen(seed);
    vector<Block>Blockchain;
    Block genesisBlock;
    genesisBlock.prevBlockHash="None";
    genesisBlock.timeStamp = time(0);
    genesisBlock.version = "v0.2";
    genesisBlock.nonce = 0;
    genesisBlock.difficultyTarget = 3;
    Blockchain.push_back(genesisBlock);
    cout << "Genesis block created..\n";
    vector <User> users;
    std::uniform_int_distribution<unsigned> distrib(0, 1000);
    std::uniform_int_distribution<unsigned> distrib1(1, 200);
    for(int i = 0; i<1000;i++){
        User client;
        client.name = "user"+to_string(i);
        client.public_key = hash_function(hash_function(client.name+to_string(client.name.length())+to_string(distrib(gen))) );
        client.balance = distrib1(gen);
        users.push_back(client);
    }
    cout << "1000 random users created..\n";
// 1000 users
    std::uniform_int_distribution<unsigned> distrib2(0, 500);
    vector<Transaction> transactionPool;
    int a = 0;
    for(int i = 0; i< 10000; i++){
        Transaction trans;
        int x = distrib(gen);
        int y = distrib(gen);
        while (y!=x)
            y = distrib(gen);
        trans.senderKey = users[x].public_key;
        trans.recipientKey = users[y].public_key;
        trans.sum = distrib2(gen);
        trans.transactionID = hash_function(trans.senderKey + trans.recipientKey + to_string(trans.sum) );
        transactionPool.push_back(trans);
    }
    cout << "10000 random transactions created..\n";
// 10000 transactions

    while(transactionPool.size()!=0){
        vector<vector<Transaction>>takenTransactions;
        int TransactionID;
        vector<vector<Transaction>> fakeTransactionPool;
        for(int j = 0; j<5;j++){
            fakeTransactionPool.push_back(transactionPool);
            vector<Transaction>emptyTransaction;
            takenTransactions.push_back(emptyTransaction);
            for(int i = 0; i<100;i++){
                std::uniform_int_distribution<unsigned> distrib3(0, fakeTransactionPool[j].size());
                int randomID = distrib3(gen);
                takenTransactions[j].push_back(fakeTransactionPool[j][randomID]);
                fakeTransactionPool[j].erase(fakeTransactionPool[j].begin()+randomID);
            }
        }
        cout << "5 different transaction pools created..\n";
        for(int j = 0; j<5;j++){
            for(int i = 0; i<100;i++){
                takenTransactions[j][i].completed=transactionConfirmation(takenTransactions[j][i], users);
            }
        }
        cout << "All taken transactions validated..\n";
        cout << "Starting to search for the new block.. \n..\n";
        Block newBlock=SearchForNextBlock(Blockchain.back(), takenTransactions, TransactionID);
        Blockchain.push_back(newBlock);
        transactionPool=fakeTransactionPool[TransactionID];
        cout << "Block completed.\n";
        cout << "Completing transactions..\n";
        for(int i = 0; i<100;i++)
            if(transactionPool[i].completed)
                completeTransaction(transactionPool[i], users);
        cout << "Completed.\n";
    }
    return 0;
}
void completeTransaction(Transaction transactionPool,vector<User>& users){
    int id1=-1,id2=-1;
    for(int i = 0; i<users.size() || (id1!=-1 && id2!=-1);i++){
        if(users[i].public_key == transactionPool.recipientKey){
            id1 = i;
        }
        if(users[i].public_key == transactionPool.senderKey){
            id2 = i;
        }
    }
    users[id1].balance+=transactionPool.sum;
    users[id2].balance-=transactionPool.sum;
}

bool transactionConfirmation(Transaction takenTransaction, vector<User>& AllUsers){
    int senderKey;
    int recipientKey;
    bool confirmSender = false;
    bool confirmRecipient = false;
    if(takenTransaction.transactionID == hash_function(takenTransaction.senderKey + takenTransaction.recipientKey + to_string(takenTransaction.sum) )){
        for(int i = 0; i<AllUsers.size();i++){
            if(takenTransaction.senderKey == AllUsers[i].public_key){
                if(AllUsers[i].balance >= takenTransaction.sum && takenTransaction.sum > 0){
                     confirmSender = true;
                }
            }
            if(takenTransaction.recipientKey == AllUsers[i].public_key){
                    confirmRecipient = true;
            }
        }
    }
    bool answer;
    if(confirmSender == true && confirmRecipient == true){
        answer = true;
    }
    else answer = false;

    return answer;
}

// Merkle Root Hash
bc::hash_digest create_merkle(bc::hash_list& merkle)
{
 // Stop if hash list is empty or contains one element
 if (merkle.empty())
 return bc::null_hash;
 else if (merkle.size() == 1)
 return merkle[0];
 // While there is more than 1 hash in the list, keep looping...
 while (merkle.size() > 1)
 {
 // If number of hashes is odd, duplicate last hash in the list.
 if (merkle.size() % 2 != 0)
 merkle.push_back(merkle.back());
 // List size is now even.
 assert(merkle.size() % 2 == 0);
 // New hash list.
 bc::hash_list new_merkle;
 // Loop through hashes 2 at a time.
 for (auto it = merkle.begin(); it != merkle.end(); it += 2)
 {
 // Join both current hashes together (concatenate).
 bc::data_chunk concat_data(bc::hash_size * 2);
 auto concat = bc::serializer<
 decltype(concat_data.begin())>(concat_data.begin());
 concat.write_hash(*it);
 concat.write_hash(*(it + 1));
 // Hash both of the hashes.
 bc::hash_digest new_root = bc::bitcoin_hash(concat_data);
 // Add this to the new list.
 new_merkle.push_back(new_root);
 }
 // This is the new list.
 merkle = new_merkle;
 }
 // Finally we end up with a single item.
 return merkle[0];
}

string getBlockMerkelRootHash(const vector<Transaction>& t){
    string zeroes = "0000000000000000000000000";
    bc::hash_list trx_hashes;
    for(auto i : t) // for each transaction 
    {
        char charTrx[65];
        strcpy(charTrx, (i.transactionID + zeroes).c_str()); // trx id and added zeroes are converted to char[] 
        trx_hashes.push_back(bc::hash_literal(charTrx)); // pushed back to hashes vector as in previous logic
    }
    const bc::hash_digest merkle_root = create_merkle(trx_hashes); // merkle tree is created
    string merkleHash = bc::encode_base16(merkle_root);
    return merkleHash;
}

vector<string> getVectorMerkelRootHash(const vector<string>& t){
    vector<string> hashes;
    for(int i = 0; i<t.size();i+=2){
        auto first = *(t.begin()+(i/2));
        auto second = *(prev(t.end())-(i/2));
        hashes.push_back(hash_function(first + second));
    }
    return hashes;
}

Block SearchForNextBlock(const Block& lastBlock, const vector<vector<Transaction>>& t, int& chosenTransaction){
    ofstream ff;
    ff.open("log.txt", std::fstream::app);
    Block block;
    chosenTransaction=0;
    vector<Transaction> bestTransactionT;
    string bestMerkelRootHash;
    uint32_t nextNonce = 0;
    int diff = lastBlock.difficultyTarget;
    block.difficultyTarget = diff;
    ///block.Transactions = t;
    ///block.merkelRootHash = getBlockMerkelRootHash(t);
    block.timeStamp = time(0);
    block.nonce = -1;
    block.version = "v0.2";
    int bestNonce;
    int bestTransaction;
    string prevBlockHash = hash_function(hash_function(to_string(lastBlock.nonce)) + lastBlock.prevBlockHash + lastBlock.merkelRootHash + to_string(lastBlock.nonce) + to_string(lastBlock.timeStamp) + lastBlock.version + to_string(lastBlock.difficultyTarget));
    int limit = 200;
    int countt = 0;
    while(countt == 0){
        countt=0;
        for(int i = 0; i<5;i++){
            bool rado = false;
            nextNonce = 0;
            block.nonce = -1;
            block.merkelRootHash = getBlockMerkelRootHash(t[i]);
            //for(int nextNonce=0;block.nonce ==-1 && nextNonce < bestNonce;nextNonce++){
                while(block.nonce == -1 && nextNonce < bestNonce && nextNonce < limit){
                    string blockHash = /*hash_function(*/hash_function(hash_function(to_string(nextNonce))+prevBlockHash + block.merkelRootHash + to_string(nextNonce) + to_string(block.timeStamp) + block.version + to_string(block.difficultyTarget))/*)*/;
                    for(int i = 0; i<diff;i++){
                        if(blockHash[i]!='a')
                            break;
                        else if (i == diff-1 && blockHash[i]=='a'){
                            block.prevBlockHash = prevBlockHash;
                            block.nonce=nextNonce;
                            ff << blockHash << "\n";
                            cout << blockHash << ":" << nextNonce << "\n";
                            rado = true;
                            break;
                        }
                    }
                    nextNonce++;
                }
            if((nextNonce-1 < bestNonce || i == 0) && rado == true){
                bestTransactionT = t[i];
                bestMerkelRootHash = block.merkelRootHash;
                bestNonce = nextNonce-1;
                bestTransaction =i;
                countt++;
                cout << "NEW best block found" << "\n";
                rado = true;
            }
        }
        if(countt==0){
            limit+=1300;
            cout << "Limit: " << limit <<"\n";
        }
    }
    block.Transactions = bestTransactionT;
    block.merkelRootHash = bestMerkelRootHash;
    chosenTransaction = bestTransaction;
    cout << "Final best block: " << bestTransaction << " with nonce: " << bestNonce << "\n";
    return block;
}


string hash_function(string fraze)
{
    string ID[5]={"01000100111110010111000000110001","11010000010010000000000000000001","01000100111110010111011110110001","11011111110010000000011000111101","11010100111110010111000100110001"};
    string hex;
    for(int i = 0; i<fraze.size();i++){
            hex += bitset<8>(fraze.c_str()[i]).to_string();
        }
        hex += "1";
        while(hex.size()%448!=0){
            hex+="0";
        }
        for(int i = 0; i<56;i++, hex+="0");
        hex += bitset<8>(fraze.size()*8).to_string();
        int dydis = hex.size()/32;
        string* hexiukai = new string[dydis];
        string zodziai[40];
        for(int i = 0;i<dydis;i++){
            hexiukai[i]=popfunction(hex);
            hex.erase(hex.begin(),hex.begin()+32);
        }
        for(int i = 0, j = 2, p = 8, k = 13, h = 0; h < 40;h++,i++,j++,p++,k++){
            if(i > dydis-1) i=0;
            if(j > dydis-1) j=0;
            if(p > dydis-1) p=0;
            if(k > dydis-1) k=0;
            zodziai[h]=xorfunction(xorfunction(xorfunction(hexiukai[k],hexiukai[p]),hexiukai[j]),hexiukai[i]);
            zodziai[h]+=zodziai[h][0];
            zodziai[h].erase(zodziai[h].begin(),zodziai[h].begin()+1);
        }
        for(int i = 0; i<5;i++){
            for(int j = 0; j<40;j++){
                if(i%2==0){
                    ID[i]=andfunction(ID[i],notfunction(zodziai[j]));
                    ID[i]+=ID[i][0];
                    ID[i]+=ID[i][1];
                    ID[i].erase(ID[i].begin(),ID[i].begin()+2);
                }
                else{
                    ID[i]=orfunction(notfunction(ID[i]),zodziai[j]);
                    ID[i]+=ID[i][0];
                    ID[i].erase(ID[i].begin(),ID[i].begin()+1);
                }
            }
        }
        stringstream ff;
        for(int i = 0; i<5;i++){
            if(ID[i][0]=='0')
                ID[i][0]='1';
            bitset<32> set(ID[i]);
            ff << std::hex << set.to_ulong();
            }
        ff >> fraze;
    return fraze;
}
string popfunction(string hex){
    hex.erase(hex.begin()+32,hex.end());
    return hex;
}

string xorfunction(string a,string b){
    string ats;
    for(int i = 0; i < 32;i++){
        if( (a[i]=='1' && b[i]=='0') ||  (b[i]=='1' && a[i]=='0') )
            ats+="1";
        else ats+="0";
    }
    return ats;
}
string andfunction(string a, string b){
    string ats;
    for(int i = 0; i < 32; i++){
        if((a[i]=='1' && b[i]=='1') || (a[i]=='0' && b[i]=='0') )
            ats+="1";
        else ats+="0";
    }
    return ats;
}
string notfunction(string a){
    string ats;
    for(int i = 0; i < 32; i++){
        if(a[i]=='1')
            ats+="0";
        else ats+="1";
    }
    return ats;
}
string orfunction(string a, string b){
    string ats;
    for(int i = 0; i < 32; i++){
        if(a[i]=='1' || b[i]=='1')
            ats+="1";
        else ats+="0";
    }
    return ats;
}



