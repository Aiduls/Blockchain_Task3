//merkle.cpp
#include <bitcoin/bitcoin.hpp>
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
 // DEBUG output -------------------------------------
 std::cout << "Current merkle hash list:" << std::endl;
 for (const auto& hash: merkle)
 std::cout << " " << bc::encode_base16(hash) << std::endl;
 std::cout << std::endl;
 // --------------------------------------------------
 }
 // Finally we end up with a single item.
 return merkle[0];
}
int main()
{
// Transactions hashes from a block (#100 000) to reproduce the same merkle root
//  bc::hash_list tx_hashes{{
//  bc::hash_literal("8c14f0db3df150123e6f3dbbf30f8b955a8249b62ac1d1ff16284aefa3d06d87"),
//  bc::hash_literal("fff2525b8931402dd09222c50775608f75787bd2b87e56995a7bdd30f79702c4"),
//  bc::hash_literal("6359f0868171b1d194cbee1af2f16ea598ae8fad666d9b012c8ed2b79a236ec4"),
//  bc::hash_literal("e9a66845e05d5abc0ad04ec80f774a7e585c6e8db975962d069a522137b80c1d"),
 }};
 
 // Transactions hashes from a new picked block (#123321) to reproduce merkle root
 bc::hash_list tx_hashes{{
 bc::hash_literal("255fb8ff561fc1668c79fbf902f394ffd1fbf29797d6d9266821ca4baef56906"),
 bc::hash_literal("551d33c93e4a7d7753b621bd275314ae4fe9741a9b4e54894d36138614d182e9"),
 bc::hash_literal("481bbbaafa310605a5cd049432c7636e1ec8008bdbb420ffacb9c4a1477f0d95"),
 bc::hash_literal("4437c011da6875608c20848f18d83beb8354314116bbf9e99caf8a20a2784aba"),
 }};
 const bc::hash_digest merkle_root = create_merkle(tx_hashes);
 std::cout << "Merkle Root Hash (encoded at base16): " << bc::encode_base16(merkle_root) << std::endl;
 //std::cout << "Merkle Root Hash-2: " << bc::encode_hash(merkle_root) << std::endl;
 return 0;
}