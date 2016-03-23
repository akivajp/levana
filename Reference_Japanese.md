# Levanaリファレンスマニュアル #

※ 現在テスト開発状態であるため、モジュール名・クラス名・メソッド名・関数名や、引数仕様などが確定しておらず、従って当リファレンスが最新版に対応される保証はなく、また今後において変更される可能性もあります。


## 【概要】 ##

Levanaはプログラミング言語Luaをベースとした汎用ライブラリLevの実行環境として実装されています。ここではLua言語の仕様は他のWebサイトに譲り、主に根幹ライブラリであるLevの使用方法をリファレンスマニュアル形式で紹介します。


## 【実行方法】 ##

Levanaを起動させるには、配布ファイルに同梱されている実行ファイル(Windowsではlevana.exe、その他の環境ではlevana)と、記述済みの初期実行スクリプト1つ(標準ではentry.lua、後述)が同一フォルダ内に配置されている必要があります。

Levanaは起動されると、初期実行スクリプトとして以下のファイルを見つけ出して、Luaスクリプトとして実行します。
  * entry.lc (コンパイル済みスクリプトとして)
  * entry.lua (テキスト形式のスクリプトとして)
  * entry.txt (テキスト形式のスクリプトとして)
該当ファイルが複数ある場合でも、最初に検出されたファイル1つのみが実行されます。


## 【外観】 ##

Levana上で実行されるLuaスクリプトは、Lua標準ライブラリの他にGUI操作などを受け持つ中核ライブラリLevをロードして使えるようになっています。

現在、以下のモジュールが使用可能となっています。
  * lev : Levana基幹ライブラリモジュール
    * [lev.app](#lev.app.md) : アプリケーション管理
    * [lev.archive](#lev.archive.md) : アーカイブ入出力モジュール
    * [lev.base](#lev.base.md) : 基底クラスモジュール
    * [lev.db](#lev.db.md) : データベースモジュール
    * [lev.font](#lev.font.md) : フォントモジュール
    * [lev.fs](#lev.fs.md) : ファイルシステムモジュール
    * [lev.image](#lev.image.md) : 画像処理モジュール
    * [lev.gl](#lev.gl.md) : CG描画モジュール
    * [lev.gui](#lev.gui.md) : GUI管理モジュール
    * [lev.input](#lev.input.md) : デバイスによる入力管理モジュール
    * [lev.locale](#lev.locale.md) : 国際化モジュール
    * [lev.prim](#lev.prim.md) : 基礎図形表現モジュール
    * [lev.net](#lev.net.md): ネットワークモジュール
    * [lev.sound](#lev.sound.md) : サウンド再生モジュール
    * [lev.timer](#lev.timer.md) : タイマーモジュール
    * [lev.util](#lev.util.md) : 便利関数モジュール

Luaスクリプト冒頭で、
```
  require("ライブラリ名")
```
とすることにより該当ライブラリと依存ライブラリが自動的に読み込まれ、対応するモジュール関数などが使用可能になります。また、以下の補助モジュールが使用可能で、多くの場合にアプリケーションの初期化を大幅に省略できます。

  * lev.std : Levana標準ライブラリ。上記の主要なモジュールを一括ロードし、アプリケーションやサウンドミキサーの初期化を行う。


---


## 【モジュールクラス・関数リファレンス】 ##

## lev.app ##
> 依存モジュール [lev](#lev.md)

  * ### appモジュール関数 ###
    * #### lev.app ####
```
  app = lev.app()
```
> > > 初回呼び出し時にはアプリケーションクラスのインスタンスを作成し、初期化してインスタンスを返します。これはアプリケーション毎に唯一(シングルトン)のオブジェクトである為、二回目以降は作成済みの同一インスタンスを返します。GUI操作に必須となるイベント処理や、デバイス入力処理を受け持つためには、このクラスが必要となります。lev.std をロードした場合、自動的にこのクラスのインスタンスが app という名前で初期化・作成されます。

  * ### appクラスメソッド ###

> > (インスタンス名を app と仮定)
    * #### app.autoloop ####
```
  app:autoloop( [top_window] )
```
> > > ライブラリが提供するイベントループを用いて、イベント同期型のループ処理を行います。 top\_window にメインウィンドウが指定された場合、メインウィンドウが閉じられると同時に関数を終了します。 main\_window が指定されないか nil の場合は、全てのウィンドウが閉じられると関数を終了します。非表示のウィンドウがある場合に、メインウィンドウが指定されていないとループから抜け出せなくなる可能性があるので、メインウィンドウを指定することを推奨します。
    * #### app.sleep ####
```
  app:sleep(delay_in_msec)
```
> > > delay\_in\_msecで指定されたミリ秒間だけ処理を停止します。
    * #### app.yield ####
```
  app:yield()
```
> > > イベントループ内において、未処理のイベントメッセージを処理させます。 [autoloop](#app.autoloop.md) を使用する場合は必要ありませんが、自前のイベントループを実装する場合は適切に yield を呼び出してイベント処理を行う必要があります。

  * ### appクラスプロパティ ###

> > (インスタンス名を app と仮定)
    * #### app.fps ####
```
  app.fps
```
> > > FPSパラメータ(Frames Per Second)を決定します。 [wait](#app.wait.md) や [run](#app.run.md) と併用して同期ループを実現させます。それ以外の方法でアプリケーションを実装する場合には必要ありません。初期値は100です。
    * #### app.interval ####
```
  app.interval
```
> > > [app.fps](#app.fps.md) と同様ですが常に、
```
  app.fps == 1000 / app.interval
  app.interval == 1000 / app.fps
```
> > > が成り立ちます。
    * #### app.title ####
```
  app.name
  app.title
```
> > > アプリケーション名を決定するプロパティです。代入と取得が可能です。初期値は"Levana Application"になっています。
    * #### app.top\_window ####
```
  app.top
  app.top_window
```
> > > トップウィンドウを決定するプロパティです。代入と取得が可能です。指定しない場合はOSがウィンドウの一つをトップウィンドウとして決定します。


---


## lev.gui ##

  * ### guiモジュール関数 ###
    * #### lev.gui.msgbox ####
```
  result = lev.gui.msgbox {
    message = "メッセージ文字列"(string),
    [ caption = "キャプション文字列"(string), ]
    [ style = "選択肢スタイル"(string), ] }
```
> > > OKボタンとメッセージのみ配置されたメッセージボックスを作成します。ユーザの注意を促すのに用います。
> > > message は表示されるメッセージを示します。省略しないで下さい。
> > > caption はメッセージボックスのキャプションです。省略された場合は 'Message' が代入されます。
> > > style は応答用のボタンを決定する文字列です。"ok|cancel", "yes|no", "yes|no|cancel" などのように指定します。

  * ### gui.frameクラス ###
    * #### lev.gui.frame ####
```
  frm = lev.gui.frame {
    [ parent = 親ウィンドウ(lev.gui.frame), ]
    [ caption = "ウィンドウキャプション"(string), ]
    [ width = 幅(number), ]
    [ height = 縦幅(number), ]
    [ x = 作成時X座標(number), ]
    [ y = 作成時Y座標(number), ]
    [ style = "スタイル指定文字列"(string), ] }
```
> > > 新規ウィンドウを作成します。非表示状態で作成されるので、GUIコントロールの設定後に show メソッドで表示する必要があります。
> > > 省略された引数はデフォルト値が用いられます。
> > > 作成失敗時には nil が返ります。
> > > styleに"fixed"を指定すると固定サイズウィンドウになります。

  * ### gui.frameクラスメソッド ###

> > (インスタンス名をfrmと仮定)
    * #### frm.close ####
```
  frm:close( [force] )
```
> > > ウィンドウを閉じる依頼イベントを送出します。
> > > force に true が指定されると強制的にウィンドウを閉じますが、
> > > 省略された場合や false の場合は、イベントハンドラに挙動が一任されます。

※ (書きかけ)

<a href='Hidden comment: 

[gui.frameクラス]

[クラスメソッド] (インスタンス名を frm と仮定)

frm:show(showing)
ウィンドウの表示/非表示を切り替えます。
showing が省略された場合はデフォルト動作として表示、true なら表示、false なら非表示になります。



【soundモジュール】

[モジュール関数]

lev.sound.play {file = ファイル名, ch = チャンネル番号}
デフォルト値 ： ch = 0
音声ファイル file を開き、チャンネル番号 ch で即時に再生を開始します。
ch のデフォルト値 0 の時はビジーチャンネルで音声を再生し、再生終了まで関数を抜けません。
即時に再生したくない場合や、細やかなエラーチェックを行いたい場合は、
loadfile やクラス作成を行って下さい。

[sound.mixerモジュール]

[モジュール関数]
mixer = lev.gui.mixer()
新規ミキサーを作成します。音楽や効果音などを同時再生したり、細やかな設定切り替えを行う場合に必要になります。
引数は必要としません。任意の引数を渡しても無視されます。

[クラスメソッド] (インスタンス名を mixer と仮定)

mixer:pause()
ミキサーを停止します。一切のチャンネルがミュートされますが、再生時間は流れている事に注意して下さい。

mixer:play()
ミキサーを有効状態にします。各チャンネルの再生状態は個別に設定する必要があります。

[プロパティ]

mixer.is_playing
ミキサーが再生状態かどうかを真偽値で返します。真偽値を代入して状態を変更することもできます。

[チャンネル操作]

mixer[ch_num]
与えられた自然数のチャンネル番号 ch_num に対応するチャンネルを返します。
存在しなければ生成してから返します。
channel に 0 が指定された場合は未割り当てのチャンネル番号を探し出して、生成してから返します。

mixer[ch_num]:open(filepath)
filepath のファイルをストリームモードで開き、ch_num 番チャンネルに割り当てます。
メモリを専有せず、すぐに音楽再生が可能な為、BGMのリアルタイム再生などに有効です。

mixer[ch_num]:load(filepath)
filepath のファイルをメモリにロードして、ch_num 番チャンネルに割り当てます。
データを完全にデコードしてからロードし、メモリを専有するので再生開始まで時間を要しますが、
ロード後はタイムラグ無くスムーズに再生可能な為、効果音や繰り返し再生したい場合などに有効です。

mixer[ch_num]:pause()
ch_num 番チャンネルを停止します。再生位置が保存されて、次回再生時には中断箇所から続行します。

mixer[ch_num]:play()
ch_num 番チャンネルを再生します。停止状態から復帰する場合は、前回の中断箇所から続行します。

mixer[ch_num]:play(filepath)
chu_num 番のチャンネルで filepath をストリームモードで開き、直ちに再生を開始します。
'></a>