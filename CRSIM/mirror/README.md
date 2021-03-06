# 操作方法

## 共通の操作
- <kbd>+</kbd> <kbd>-</kbd>：盤面のマス数を変更 (盤面は初期化され，コマンドの実行履歴も削除される)
- <kbd>Shift</kbd> + <kbd>BackSpace</kbd>：盤面を初期化 (コマンドの実行履歴も削除される)
- <kbd>R</kbd>：コマンドをすべて戻す
- マスをクリック：マスを選択し，編集モードに切り替える
- マス以外の部分をクリック：編集モードを解除する
- 右側をスクロール：コマンドの実行履歴を表示
- ファイルをドラッグ＆ドロップ：ファイルから盤面を読み込む
- <kbd>O</kbd>：盤面を読み込むファイルを選択
- <kbd>P</kbd>：盤面を保存
- <kbd>M</kbd>：コマンド列をプログラムとして保存

## 実行モード

デフォルトで設定されているモード．コマンドの入力，実行，削除などができる．

- <kbd>0</kbd> ~ <kbd>9</kbd>：色を選択
    - 0: 白，1: 黒，2: 赤，3: 橙，4: 黄，5: 緑，6: 水，7: 青，8: 紫，9: 桃 に対応
- <kbd>↓</kbd> <kbd>→</kbd> <kbd>↑</kbd> <kbd>←</kbd> / <kbd>S</kbd> <kbd>D</kbd> <kbd>W</kbd> <kbd>A</kbd>：選択した色を基準に各方向に盤面を回転
- <kbd>BackSpace</kbd>：コマンドを 1 手戻す
- <kbd>E</kbd>：編集モードに切り替え

## 編集モード

盤面を自由に編集できるモード．

- <kbd>0</kbd> ~ <kbd>9</kbd>：選択中のマスを指定した色に変更 (上の対応色を参照)
- <kbd>BackSpace</kbd>：選択中のマスを白に変更
- <kbd>↓</kbd> <kbd>→</kbd> <kbd>↑</kbd> <kbd>←</kbd> / <kbd>S</kbd> <kbd>D</kbd> <kbd>W</kbd> <kbd>A</kbd>：選択するマスを変更
- <kbd>E</kbd>：編集モードを終了

# ライセンス

- [LICENSE.html](./LICENSE.html)

# 未実装機能

- 編集の Undo