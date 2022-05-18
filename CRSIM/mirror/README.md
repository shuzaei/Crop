# 操作方法

## 共通の操作
- <kbd>+</kbd> <kbd>-</kbd>：盤面のマス数を変更 (盤面は初期化され，コマンドの実行履歴も削除される)
- <kbd>R</kbd>：盤面を初期化 (コマンドの実行履歴も削除される)
- マスをクリック：マスを選択し，編集モードに切り替える
- 右側をスクロール：コマンドの実行履歴を表示

## 実行モード

デフォルトで設定されているモード．コマンドの入力，実行，削除などができる．

- <kbd>0</kbd> ~ <kbd>9</kbd>：色を選択
    - 0: 白，1: 黒，2: 赤，3: 橙，4: 黄，5: 緑，6: 水，7: 青，8: 紫，9: 桃 に対応
- <kbd>↓</kbd> <kbd>→</kbd> <kbd>↑</kbd> <kbd>←</kbd> / <kbd>s</kbd> <kbd>d</kbd> <kbd>w</kbd> <kbd>a</kbd>：選択した色を基準に各方向に盤面を回転
- <kbd>BackSpace</kbd>：コマンドを 1 手戻す
- <kbd>Shift</kbd> + <kbd>BackSpace</kbd>：コマンド全消去
- <kbd>E</kbd>：編集モードに切り替え

## 編集モード

盤面を自由に編集できるモード．

- <kbd>0</kbd> ~ <kbd>9</kbd>：選択中のマスを指定した色に変更 (上の対応色を参照)
- <kbd>↓</kbd> <kbd>→</kbd> <kbd>↑</kbd> <kbd>←</kbd> / <kbd>s</kbd> <kbd>d</kbd> <kbd>w</kbd> <kbd>a</kbd>：選択するマスを変更
- <kbd>E</kbd>：編集モードを終了

# ライセンス

- [LICENSE.html](./LICENSE.html)