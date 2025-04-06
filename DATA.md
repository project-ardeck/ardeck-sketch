# データ形式について 0.2.0
Ardeckの通信は、最小限のデータ送信ですむように、数バイトに様々なデータをまとめている。
また、実際に送信されるデータはCOBSに従ってエンコードされており、データ

## 構造
COBSに従ってエンコードされたデータは、以下のようなデータ構造です

|1Byte [MSB]|1Byte or 2Byte|1Byte|1Byte|
| --- | --- | --- | --- |
|_FOR_COBS_|__DATA__|_CHECK_SUM_|_0x00_|


`DATA` は、場合によって長さが異なり、送信したいスイッチの種類に依存します：

- デジタルスイッチの場合、 `DATA` は1Byteになります
- アナログスイッチの場合、 `DATA` は2Byteになります

### デジタルスイッチの `DATA`

| 1bit(MSB) | 6bit | 1bit(LSB) |
| --- | --- | --- |
| `Digital` or `Analog` | Pin number | `HIGH` or `LOW` |

### アナログスイッチの `DATA`

| 1bit(MSB) | 5bit | 10bit(LSB) |
| --- | --- | --- |
| `Digital` or `Analog` | Pin number | `0 [DEC]` to `1023 [DEC]`  |
