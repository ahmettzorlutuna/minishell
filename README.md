# Proje Gereksinimleri

## Temel Gereksinimler

### Çalışma Geçmişi
Komutları hatırlamalı ve tekrar kullanabilmelisiniz.

### Yürütülebilir Dosya Bulma ve Çalıştırma
Girilen komutu bir yürütülebilir dosya olarak aramalı.

- Aramayı `PATH` ortam değişkenindeki dizinlerde yapmalı.
- Göreceli (örneğin `./program`) veya mutlak (örneğin `/bin/ls`) yollarla belirtilen dosyaları da çalıştırmalı.

### Sinyal İşleme
Alınan bir sinyali (örneğin Ctrl+C) belirtmek için en fazla bir adet global değişken kullanmalısınız.

- Bu global değişken sadece sinyal numarasını saklamalı, başka hiçbir bilgiye erişim sağlamamalıdır.
- Karmaşık veri yapılarını globalde tutmak yasaktır.

### Tırnak İşaretleri
- Tek tırnak (`'`) içindeki metakarakterler (örneğin `*`, `>`, `<`) yorumlanmamalıdır.
- Çift tırnak (`"`) içindeki metakarakterler yorumlanmamalıdır, ancak dolar işareti (`$`) özel bir durumdur (ortam değişkeni genişletilmesi için).

### Yönlendirmeler (Redirection)

- `<`: Standart girdi dosyasını değiştirmeli (bir dosyadan okuma).
- `>`: Standart çıktıyı bir dosyaya yönlendirmeli (dosyaya yazma, dosya yoksa oluşturmalı, varsa üzerine yazmalı).
- `<<`: "Here Document" olarak bilinir. Belirtilen bir sınırlayıcıya kadar olan girdiyi komuta standart girdi olarak vermelidir. Geçmişe kaydedilmesine gerek yoktur.
- `>>`: Standart çıktıyı bir dosyanın sonuna eklemeli (dosyaya yazma, dosya yoksa oluşturmalı, varsa sonuna eklemeli).
- `|`: Boru (Pipe) oluşturmalı. Bir komutun çıktısını diğer komutun girdisine bağlamalıdır.

### Ortam Değişkenleri
- `$` ile başlayan ve ardından karakterler gelen ifadeler ortam değişkenlerinin değerleriyle değiştirilmelidir (genişletilmelidir).
- `$?` ifadesi, en son çalıştırılan ön plan (foreground) boru hattının (pipeline) çıkış durumunu (exit status) göstermelidir.

### Kontrol Tuşları (Ctrl+C, Ctrl+D, Ctrl-)
Bash benzeri davranış sergilemelidir.

- **İnteraktif Modda** (Komut satırının kullanıcıdan girdi beklediği durum):
  - **Ctrl+C**: Yeni bir satırda yeni bir komut istemcisi (prompt) göstermelidir.
  - **Ctrl+D**: Shell'den çıkmalıdır.
  - **Ctrl+**: Herhangi bir işlem yapmamalıdır.

### Dahili Komutlar (Built-in Commands)

- `echo [-n] [argümanlar...]`: Metin yazdırmalıdır. `-n` seçeneği ile sondaki yeni satır karakterini yazdırmamalıdır.
- `cd [yol]`: Mevcut çalışma dizinini (current working directory) belirtilen göreceli veya mutlak yola değiştirmelidir.
- `pwd`: Mevcut çalışma dizininin tam yolunu yazdırmalıdır. Herhangi bir seçeneği olmamalıdır.
- `export`: Herhangi bir seçeneği olmamalıdır (genellikle ortam değişkenlerini dışarı aktarmak için kullanılır, ancak bu gereksinimde seçenek belirtilmemiş).
- `unset [değişken_adı]`: Belirtilen ortam değişkenini silmelidir. Herhangi bir seçeneği olmamalıdır.
- `env`: Mevcut ortam değişkenlerini listelemelidir. Herhangi bir seçeneği veya argümanı olmamalıdır.
- `exit`: Shell'den çıkmalıdır. Herhangi bir seçeneği olmamalıdır.

### Ek Notlar
- `readline()` fonksiyonunun bellek sızıntısına neden olabileceği belirtilmiş, ancak bunu düzeltmeniz beklenmiyor. Ancak, kendi yazdığınız kodda bellek sızıntısı olmamalıdır.
- Konuyla ilgili olmayan özel karakterler veya tamamlanmamış tırnaklar yorumlanmamalıdır.


## Edge Case'ler

### Tırnak İşlemleri
- Tırnaklar açılıp kapanmadığında hata verilmelidir. Eğer tırnaklar tamamlanmazsa bir hata mesajı gösterilmelidir. (İnteraktif mod dışında).

### Export Ortam Değişkeni
- `export` komutunda değişken atarken, değişken adı sayı ile başlayamaz. Değişken adı sadece harf veya alt tire ile başlayabilir. Bu kurallara uymayan girişlerde hata verilmelidir.

### `cd` Komutu
- `cd -` komutu, bir önceki konuma geri dönmelidir.
- `cd | echo hi` gibi komutlarda `cd` çalışmaz çünkü bir pipe komutu vardır. `cd` yalnızca parent process'te (üst düzey süreçte) yolu değiştirebilir.

### `cat` Komutu ve `Here Document`
- `cat << eof` kullanıldığında, `eof` sınırlayıcıyı doğru şekilde işlemek için kontrol yapılmalıdır. `eof` yazılınca işlem durmamalıdır.

### Ctrl Tuşları
- **Ctrl+C**: Yeni bir satırda yeni bir komut istemcisi (prompt) göstermelidir.
  - **`cat` komutunda** Ctrl+C, `cat`'ten çıkmalıdır.
- **Ctrl+D**: Shell'den çıkmalıdır.
  - **`cat` komutunda** Ctrl+D, programdan çıkmalıdır.
- **Ctrl+**: Herhangi bir işlem yapılmamalıdır.

### Tek Tırnaklar
- Tek tırnak içindeki metakarakterler (örneğin `*`, `>`, `<`) yorumlanmamalıdır, bu yüzden tek tırnaklarda bir işlem yapılmasına gerek yoktur.

### Sinyal Çıkışı
- Çıkış kodu için sinyallerde `128 + sinyal numarası` kullanılır. Örneğin, bir sinyal nedeniyle çıkıldığında çıkış kodu 130 (`128 + 2`) olacaktır.

### `echo` Komutu
- `echo "hi"umut'efe'` gibi yanlış tırnak kullanımında hata verilmelidir.
- `echo hi | (Hata)` şeklindeki komutlar doğru çalışmamalıdır.

### Çıkış Durumu
- `echo $?` çıktısı doğru şekilde döndürülmelidir. Örneğin, `echo $?ü` gibi yanlış bir yazımda `0` (başarılı) yerine hata kodu döndürülmelidir.

### Exit Komutu
- `exit` komutuyla çıkarken, çıkış modülünü 256 olarak almalıdır.

### Diğer Durumlar
- `echo -nnnnnn -nnnnnnn kekfkejejw` gibi hatalı girişlerde uygun hata mesajı verilmelidir.
- `echo ""` gibi hatalı girişlerde de uygun hata verilmelidir.