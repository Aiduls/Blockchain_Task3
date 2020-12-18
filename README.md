# Blockchain_Task3
### Libbitcoin dalis

1. Libbitcoin biblioteka C++ kalbai buvo instaliuota Linux Ubuntu OS, naudojant Ubuntu programą Windows sistemai.

2. Naudojant minėtą Ubuntu programą Windows'ams, sukompiliuota ir paleista užduotyje duota createMerkle() funkcija. 

   Pirmos keturios bloko transakcijos paimtos iš 123321-ojo bloko. Jų informaciją galima rasti [čia](https://www.blockchain.com/btc/block/123321) .

3. Kadangi mano antroji užduotis buvo daroma C# kalba, Blockchain programos antrąją versiją pasiskolinau iš [Egidijaus G.](https://github.com/3gis/Blockchain/tree/V0.2) 

   Joje, sutvarkius tam tikras kompiliatoriaus klaidas,  buvo sukurta logika, implementuojanti duotąją createMerkle() funkciją.

   Šią versiją galima rasti faile `blockchainc++.cpp`

### Python-bitcoinlib dalis

1. Bitcoin mazgo aplinkoje buvo išbandytos teoriniai komandų pavyzdžiai `getblockchaininfo, getrawtransaction, decoderawtransaction`

`, getblockhash, getblock`

2. Python kalba realizuota programa `1uzduotis.py`, skaičiuojanti bet kokios įvestos Bitcoin transakcijos mokestį. Programos kode po komentaru taip pat yra įdėtas sakinys, leidžiantis pasižiūrėti koks mokestis buvo skirtas vienai vertingiausių transakcijų.