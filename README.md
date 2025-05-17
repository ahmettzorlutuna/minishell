Temel Gereksinimler:
Çalışma Geçmişi: Komutları hatırlamalı ve tekrar kullanabilmelisiniz.
Yürütülebilir Dosya Bulma ve Çalıştırma:Girilen komutu bir yürütülebilir dosya olarak aramalı.
Aramayı PATH ortam değişkenindeki dizinlerde yapmalı.
Göreceli (örneğin ./program) veya mutlak (örneğin /bin/ls) yollarla belirtilen dosyaları da çalıştırmalı.
Sinyal İşleme:Alınan bir sinyali (örneğin Ctrl+C) belirtmek için en fazla bir adet global değişken kullanmalısınız.
Bu global değişken sadece sinyal numarasını saklamalı, başka hiçbir bilgiye erişim sağlamamalıdır. Bu, sinyal işleyicinizin ana program verilerine doğrudan erişmesini engellemek içindir. Karmaşık veri yapılarını globalde tutmak yasaktır.
Tırnak İşaretleri:Tek tırnak (') içindeki metakarakterler (örneğin *, >, <) yorumlanmamalıdır.
Çift tırnak (") içindeki metakarakterler yorumlanmamalıdır, ancak dolar işareti ($) özel bir durumdur (ortam değişkeni genişletmesi için).
Yönlendirmeler (Redirection):
<: Standart girdi dosyasını değiştirmeli (bir dosyadan okuma).
>: Standart çıktıyı bir dosyaya yönlendirmeli (dosyaya yazma, dosya yoksa oluşturmalı, varsa üzerine yazmalı).
<<: "Here Document" olarak bilinir. Belirtilen bir sınırlayıcıya kadar olan girdiyi komuta standart girdi olarak vermelidir. Geçmişe kaydedilmesine gerek yoktur.
>>: Standart çıktıyı bir dosyanın sonuna eklemeli (dosyaya yazma, dosya yoksa oluşturmalı, varsa sonuna eklemeli).
|: Boru (Pipe) oluşturmalı. Bir komutun çıktısını diğer komutun girdisine bağlamalıdır.
Ortam Değişkenleri:
$ ile başlayan ve ardından karakterler gelen ifadeler ortam değişkenlerinin değerleriyle değiştirilmelidir (genişletilmelidir).
$? ifadesi, en son çalıştırılan ön plan (foreground) boru hattının (pipeline) çıkış durumunu (exit status) göstermelidir.
Kontrol Tuşları (Ctrl+C, Ctrl+D, Ctrl-):
Bash benzeri davranış sergilemelidir.
İnteraktif Modda: (Komut satırının kullanıcıdan girdi beklediği durum)
Ctrl+C: Yeni bir satırda yeni bir komut istemcisi (prompt) göstermelidir.
Ctrl+D: Shell'den çıkmalıdır.
Ctrl+: Herhangi bir işlem yapmamalıdır.
Dahili Komutlar (Built-in Commands):
echo [-n] [argümanlar...]: Metin yazdırmalıdır. -n seçeneği ile sondaki yeni satır karakterini yazdırmamalıdır.
cd [yol]: Mevcut çalışma dizinini (current working directory) belirtilen göreceli veya mutlak yola değiştirmelidir.
pwd: Mevcut çalışma dizininin tam yolunu yazdırmalıdır. Herhangi bir seçeneği olmamalıdır.
export: Herhangi bir seçeneği olmamalıdır (genellikle ortam değişkenlerini dışarı aktarmak için kullanılır, ancak bu gereksinimde seçenek belirtilmemiş).
unset [değişken_adı]: Belirtilen ortam değişkenini silmelidir. Herhangi bir seçeneği olmamalıdır.
env: Mevcut ortam değişkenlerini listelemelidir. Herhangi bir seçeneği veya argümanı olmamalıdır.
exit: Shell'den çıkmalıdır. Herhangi bir seçeneği olmamalıdır.
Ek Notlar:
readline() fonksiyonunun bellek sızıntısına neden olabileceği belirtilmiş, ancak bunu düzeltmeniz beklenmiyor. Ancak, kendi yazdığınız kodda bellek sızıntısı olmamalıdır.
Konuyla ilgili olmayan özel karakterler veya tamamlanmamış tırnaklar yorumlanmamalıdır.

**************************************************************************************************

Tırnak açılıp kapanmadıysa handle’a gerek yok. Hata verdir. İnteractive mod harici.
Export değişken atarken sayı ile başlayamaz. Alt tire yada harf. Diğer türlü hata verdir.
cd - bir önceki konuma geri döner
cat << eof”” handle la. eof yazınca yine de duracak
<< eof
cd | echo hi
cd çalışmaz çünkü pipe var. cd sadece parentta yolu değiştirebilir.
echo -nnnnnn -nnnnnnn kekfkejejw
Ctrl c alt satıra geç. Ctrl d programdan çık
Cat için Ctrl c cat’ten çık. Ctrl d programdan. \ bir şey yapma
Sadece Tek tırnak handle gerek yok
Sinyallerde çıkış için 128 + 2
echo “hi”umut’efe’
echo hi | (Hata)
echo $?ü çıktı: 0ü
“” hata
Exit ile çıkarken modülünü al 256