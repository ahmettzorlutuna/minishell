# minishell
1. Genel Gereksinimler
✅ Shell geçmişi (history) tutulmalı.

✅ Komutlar doğru yürütülebilir dosya olarak bulunmalı ve çalıştırılmalı:

PATH değişkeni aracılığıyla aranmalı.

Göreli (./program) veya mutlak yol (/bin/ls) desteklenmeli.

✅ Sadece bir adet global değişken kullanılabilir:

Bu değişken yalnızca alınan sinyal numarasını tutmalıdır.

Ana veri yapılarına erişmemeli.

“norm” tipi struct’lar global olarak kullanılmamalı.

2. Alıntı Karakterleri (Quotes)
✅ Tek tırnak ' içindeki karakterler kesinlikle yorumlanmamalı.

Örn: 'echo $HOME' → $HOME değişkeni genişletilmez.

✅ Çift tırnak " içindekiler yorumlanır ama yalnızca $ karakteri:

Yani $VAR ifadesi genişletilir, diğer özel karakterler (örneğin |, *, >) yorumlanmaz.

Örn: "hello $USER" → hello kullanıcıadı

3. Yönlendirmeler (Redirections)
✅ < → Girdi yönlendirmesi

✅ > → Çıktı yönlendirmesi (dosyayı ezerek yazar)

✅ >> → Çıktı yönlendirmesi (dosyaya ekleme yapar)

✅ << → Heredoc: Belirli bir sınırlayıcıya kadar kullanıcıdan giriş alır. (history’ye eklenmesi gerekmez)

4. Pipe (|)
✅ Komutlar | ile birbirine bağlanabilmeli.

Örn: ls | grep .c | wc -l

5. Değişken Genişletmeleri
✅ $VAR → Ortam (environment) değişkenleri genişletilmeli.

✅ $? → En son çalıştırılan foreground pipeline’ın çıkış durumu döndürülmeli.

6. Klavye Kısayolları (Signals)
İnteraktif modda (terminal üzerinden kullanıcı komut girerken):
✅ Ctrl-C → Yeni bir satırda yeni prompt göstermeli.

✅ Ctrl-D → Shell’den çıkmalı.

✅ Ctrl-\ → Hiçbir şey yapmamalı.

💡 İnteraktif mod demek: Kullanıcının terminalde aktif olarak komut yazdığı mod. Yani shell başlatıldığında prompt çıktığında girilen komutlar bu moddadır.

7. Yerleşik Komutlar (Builtins)
Aşağıdaki komutlar shell tarafından direkt desteklenmeli ve execve() ile çalıştırılmamalı:

Komut	Açıklama
echo -n	Sonuna yeni satır koymadan yazı yazdırır
cd	Dizin değiştirir (yalnızca yol parametresi alır)
pwd	Geçerli dizini gösterir
export	Ortam değişkeni ekler (opsiyonsuz)
unset	Ortam değişkeni siler (opsiyonsuz)
env	Ortam değişkenlerini listeler
exit	Shell'den çıkar (opsiyonsuz)

8. Bellek Yönetimi
✅ Kendi yazdığın kodda bellek sızıntısı olmamalı.

⚠️ readline() fonksiyonu sızıntı yapabilir, bunu düzeltmen gerekmiyor.
