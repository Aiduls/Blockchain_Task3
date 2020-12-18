from bitcoin.rpc import RawProxy

p = RawProxy()

trx = raw_input("Enter the ID of transaction: ")

# Uncomment the following line if need to check the special transaction.
# trx = "4410c8d14ff9f87ceeed1d65cb58e7c7b2422b2d7529afc675208ce2ce09ed7d"

# Gets raw trx in hex
rawTrx = p.getrawtransaction(trx)

# Decodes trx into a JSON object
decodedTrx = p.decoderawtransaction(rawTrx)

sumIn = 0
sumOut = 0

for input in decodedTrx['vin']:
    vinTrxId = input['txid']
    rawVinTrx = p.getrawtransaction(vinTrxId)
    decodedVinTrx = p.decoderawtransaction(rawVinTrx)
    vinOut = input['vout']
    for output in decodedVinTrx['vout']:
        if(vinOut == output['n']):
            sumIn = sumIn + output['value']

for output in decodedTrx['vout']:
    sumOut = sumOut + output['value']

print(sumIn - sumOut)