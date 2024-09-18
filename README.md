# Maybe_a_vectorized_ford_johnson
```bash
c++ FJmerge.cpp -mavx2  -msse2 -std=c++98 -D DEBUG
```
# example :
```bash
./a.out `shuf -i 1-3000 -n 3000 | tr "\n" " "` 
```
