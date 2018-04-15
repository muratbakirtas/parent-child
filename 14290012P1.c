

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>


void reverse(const char * const sPtr){
	if(sPtr[0]=='\0')
		return;
	else{
		reverse(&sPtr[1]);
		putchar(sPtr[0]);
	}
}


int main()
{

    // iki pipe kullanıyoruz.
    // ilk pipe parent'a input gönderir.
    // ikinci pipe child'e birleştirilmiş dizi gönderir. 
 
    int fd1[2];  // pipe için iki indisli birinci diziyi tanımladık.
    int fd2[2];  //pipe için iki indisli ikinci diziyi tanımladık.
 
    
    char input_str[100]; // input dizisini tanımladık.
    pid_t p; // process id türünde değişken tanımladık.
 
    if (pipe(fd1)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    if (pipe(fd2)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    // yukarıda pipe'lar oluşturuldu ve pipe'lar '-1' değerini alırsa ekrana hata bildirdik.

    p = fork();	//fork çalıştırıyoruz.
 
    if (p < 0)
    {
        fprintf(stderr, "fork Failed" ); // pid '-1' döndüyse fork hata bildirir.
        return 1;
    }
 
    else if (p > 0) // pid 0 dan büyük olursa parent process çalışır.
    {
	scanf("%s", input_str); // kullanıcıdan input değeri istedik.
        char concat_str[100];
 	
        close(fd1[0]);  // birinci pipe için okuma işlemi biter.
 
        write(fd1[1], input_str, strlen(input_str)+1);
        close(fd1[1]);  // input dizisini yazar ve birinci pipe için yazma işlemi bitirilir.
 
        wait(NULL); // child'in dizi göndermesi beklenir.
 
        close(fd2[1]); // ikinci pipe'n yazma işlemi bitirilir.

        // child diziyi okur, ekrana bastırır ve okuma işlemi biter.
        read(fd2[0], concat_str, 100); 
        printf("   %s\n", concat_str); 
        close(fd2[0]); // child diziyi okur, ekrana bastırır ve okuma işlemi biter.
    }
 
    else // burada child process calışır.
    {
        close(fd1[1]);  // birinci pipe için yazma işlemi biter.
 
        // birinci pipe kullanarak diziyi okur.
        char concat_str[100];
        read(fd1[0], concat_str, 100);
 
        // diziyi birleştirir.
        int k = strlen(concat_str);
        int i;
       
	reverse(concat_str); // reverse fonksiyonunu çalıştırdık.
		
 
        concat_str[k] = '\0';   // dizi /0 ile biter
 
        // iki okuma işlemide biter.
        close(fd1[0]);
        close(fd2[0]);
 
        // diziyi birleştirerek sonlandırır.
        write(fd2[1], concat_str, strlen(concat_str)+1);
        close(fd2[1]);
 
        exit(0);
    }
}
