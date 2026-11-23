# Run
```sh
cmake -S . -B out/build/release -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$HOME/.local/rpggame-deps;/opt/homebrew"
cmake --build out/build/release --target RPGgame --parallel
./out/build/release/RPGgame
```
