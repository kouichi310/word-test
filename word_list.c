#include <stdio.h>
#include <stdlib.h>
#define LENGTH_OF_WORD 20
#define TOTAL_LIST_LENGTH 500
struct list
{
    int num;/*番号*/
    char en_word[LENGTH_OF_WORD];/*英単語*/
    char jp_word[LENGTH_OF_WORD];/*日本語訳*/
    int correct;/*正解なら０、不正解なら１*/
};

void lower(char words[LENGTH_OF_WORD]){
    //文字列をすべて小文字にする関数
    int i;
    for(i=0;words[i]!='\0';i++){
        if('A' <= words[i] && words[i] <= 'Z')words[i]=words[i]+('a' - 'A');
    }
}
void string_copy(char original[LENGTH_OF_WORD],char copy[LENGTH_OF_WORD]){
    //originalをコピーに返す関数
    int n;
    for(n=0;original[n]!='\0';n++)copy[n]=original[n];
    copy[n]='\0';
} 
int string_length(char a[]){
    //文字列の長さを返す関数
    int n;
    for(n=0;a[n]!='\0';n++);
    return n;
}
int int_length(int a[TOTAL_LIST_LENGTH]){
    //文字列の長さを返す関数
    int n;
    for(n=0;n<TOTAL_LIST_LENGTH;n++);
    return n;
}
int alphabet_or_not(char x[LENGTH_OF_WORD]){
    //アルファベットならちがうなら1
    int i,j=0;
    j=string_length(x);
    for(i=0;i<j;i++){
        if(x[i]>='a'&& x[i]<='z'|| x[i]>='A' && x[i]<= 'Z')return 0;
    }
    return 1;
}
int string_cmp(char s[LENGTH_OF_WORD],char t[LENGTH_OF_WORD]){
    //sとtが同じ文字列なら0、ちがければ1の関数
    int n;
    if(string_length(s)!=string_length(t))return 1;
    for(n=0;s[n]!='\0';n++)if(s[n]!=t[n])return 1;
    return 0;
} 
int search_arrey(int x,int y[TOTAL_LIST_LENGTH]){
    //配列yにxがあるか調べる関数あれば1、無ければ0
    int i,j;
    for(i=0;i<TOTAL_LIST_LENGTH;i++){
        if(x==y[i])return 1;
    }
    return 0;
}
int search_list(struct list word[TOTAL_LIST_LENGTH],char target[LENGTH_OF_WORD]){
    //構造体配列のなかの英単語にtargetがあるか調べる関数あればその番号＋１なければ0
    int i,ans,j;
    for(i=0;word[i].en_word[0]!='\0';i++){
        ans=0;
        for(j=0;word[i].en_word[j]!='\0';j++){
            if(word[i].en_word[j]==target[j])ans++;
        }
        if(ans==string_length(target))return i+1;
    }
    return 0;
}
void swap_string(char string1[LENGTH_OF_WORD],char string2[LENGTH_OF_WORD]){
    //st1とst2の入れ替え用関数
    char tmp[LENGTH_OF_WORD];
    string_copy(string1,tmp);
    string_copy(string2,string1);
    string_copy(tmp,string2);
}
void move_list(struct list word[TOTAL_LIST_LENGTH]){
    //構造体配列[n]を[n+1]に移動する関数
    int n,i=0;
    for(n=0;word[n].num!=0;n++);
    for(;0<=n;n--){
        word[n].num=word[n-1].num+1;
        string_copy(word[n].en_word,word[n+1].en_word);
        string_copy(word[n].jp_word,word[n+1].jp_word);
        word[n].correct=word[n-1].correct;
    }
}
void add_list(struct list word[TOTAL_LIST_LENGTH],char en_wo[LENGTH_OF_WORD],char jp_wo[LENGTH_OF_WORD]){
    //構造体配列の0番に単語を追加する関数
    int i;
    move_list(word);
    word[0].num=1;
    string_copy(en_wo,word[0].en_word);
    string_copy(jp_wo,word[0].jp_word);
    word[0].correct=1;
}
int com_char(char string1[LENGTH_OF_WORD],char string2[LENGTH_OF_WORD]){
    //st1が大きければ1,小さければ-1
    int i;
    if(string_cmp(string1,string2)==0)return 0;
    for(i=0;string1[i]!='\0' || string2[i]!='\0';i++){
        if(string1[i] < string2[i])return 0;
        if(string1[i] > string2[i])return -1;
    }
}
void sort_list(struct list word[TOTAL_LIST_LENGTH]){
    //構造体配列を英単語部分でソートするプログラム
    int i,list_num=0,ans=0,ok=0,tmp=0;
    for(i=0;word[i].en_word[0]!='\0';i++)
    list_num=i;
    while(1){
        for(i=0;list_num>i;i++){
            ans=com_char(word[i].en_word,word[i+1].en_word);
            if(ans<0){
                swap_string(word[i].en_word,word[i+1].en_word);
                swap_string(word[i].jp_word,word[i+1].jp_word);
                tmp = word[i+1].correct;
                word[i+1].correct=word[i].correct;
                word[i].correct= tmp;
            }
            if(ans==0)ok++;
        }
        if(ok>list_num)break;
    }
}
void struct_write(struct list out_strct[TOTAL_LIST_LENGTH]){
    //xmlに構造体を保存する関数。"番号.英単語,日本語訳,correct"のカンマ区切りで出力
    //correctは正解したら0になるようにし、はじめは1で登録される
    sort_list(out_strct);
    int i;
    FILE *fp;
    if((fp = fopen("word_list.csv", "w" )) == NULL ) {
        printf( "ファイルがオープンできませんでした\n" );
        exit( 1 );
    }
    for(i=0;out_strct[i].en_word[0]!='\0';i++){
        fprintf( fp,"%d,%s,%s,%d\n",out_strct[i].num,out_strct[i].en_word,out_strct[i].jp_word,out_strct[i].correct);
    }
    fprintf(fp,"%d",TOTAL_LIST_LENGTH+1);
}
void struct_read(struct list in_strct[TOTAL_LIST_LENGTH]){
    //xmlから構造体を取得する関数
    int i=0,num=0,correct=0;
    char en_wo[LENGTH_OF_WORD],jp_wo[LENGTH_OF_WORD];
    FILE *fp;
    if((fp = fopen("word_list.csv", "r" )) == NULL ) {
        printf( "ファイルがオープンできませんでした\n" );
        exit( 1 );
    }
    for(i=0;;i++){
        fscanf( fp,"%d,%[^,],%[^,],%d",&num,en_wo,jp_wo,&correct);
        if(num==TOTAL_LIST_LENGTH+1)break;
        in_strct[i].num=num;
        string_copy(en_wo,in_strct[i].en_word);
        string_copy(jp_wo,in_strct[i].jp_word);
        in_strct[i].correct=correct;
    }
    fclose( fp );
}
void add_word(struct list word[TOTAL_LIST_LENGTH]){
    //単語帳に単語を追加する関数
    int i=0;
    printf("単語追加モード\n");
    char en_word[LENGTH_OF_WORD]="",jp_word[LENGTH_OF_WORD]="";
    while(1){
            printf("英単語を入力してください\n");
            scanf("%s",en_word);
            printf("日本語訳を入力してください\n");
            scanf("%s",jp_word);
            lower(en_word);
            if(search_list(word,en_word)!=0){
                printf("同じ単語は登録できません\n");
            }else{
                if(alphabet_or_not(en_word)==0){
                    add_list(word,en_word,jp_word);
                }else{
                    printf("英字以外が入っています\n");
                }
            }
            printf("まだ続ける場合は1を入力してください。\n");
            scanf("%d",&i);
            if(i!=1)break;
        }
}
int random_get(int min,int max){
    //minからmaxの範囲でランダム生成
    return min + rand()%max+1;
}
int test_all(struct list word[TOTAL_LIST_LENGTH]){
    //テスト用関数
    char ans[LENGTH_OF_WORD];
    int cor_num=0,per=0,i,random_max=0,j=0,k;
    int make_random[TOTAL_LIST_LENGTH]={};
    for(i=0;word[i].en_word[0]!='\0';i++)
    random_max=i;
    printf("練習(すべて)スタート！\n");
    for(i=0;i<random_max;i++){
        for(k=0;search_arrey(j,make_random)!=0;k++){
            j=random_get(0,random_max+1);
        }
        make_random[i]=j;
        printf("次の英語訳は何かな？\n%s\n",word[j].jp_word);
        scanf("%s",ans);
        lower(ans);
        if(string_cmp(ans,word[j].en_word)==0){
            printf("正解！\n");
            word[j].correct=0;
            cor_num++;
        }else{
            printf("ざんねーん!正解は%sでした！\n",word[j].en_word);
            word[j].correct=1;
        }
    }
    per=cor_num*100/i;
    printf("おしまい！\n正答率は%d％でした！\n",per);
    return 0;
}
int test_miss_only(struct list word[TOTAL_LIST_LENGTH]){
    //間違えたもののみの関数
    char ans[LENGTH_OF_WORD];
    int make_random[TOTAL_LIST_LENGTH]={};
    int cor_num=0,qe=0,i,max_random=0,k,j=0;
    for(i=0;word[i].en_word[0]!='\0';i++){
        if(word[i].correct==1)qe++;
    }
    if(qe==0){
        printf("ミスはなかったよ\n");
        return 0;
    } 
    max_random=i;
    printf("練習(ミスだけ)スタート！\nミスの数は%dだよ！\n\n",qe);
    qe=0;
    for(i=0;word[i].en_word[0]!='\0';i++){
        for(k=0;search_arrey(j,make_random)!=0;k++){
            j=random_get(0,max_random+1);
        }
        make_random[i]=j;
        if(word[j].correct==1){
            qe++;
            printf("次の英語訳は何かな？\n%s\n",word[j].jp_word);
            scanf("%s",ans);
            lower(ans);
            if(string_cmp(ans,word[j].en_word)==0){
                printf("正解！\n");
                word[j].correct=0;
                cor_num++;
            }else{
                printf("ざんねーん!正解は%sでした！\n",word[j].en_word);
                word[j].correct=1;
            }
        }
    }
    printf("おしまい！\n正答率は%d％でした！\n",cor_num*100/qe);
}
int search_word(struct list word[TOTAL_LIST_LENGTH]){
    //日本語訳サーチ用の関数
    printf("日本語訳を探します。\n");
    char s[LENGTH_OF_WORD];
    int j=0,i=1;
    switch(i){
        case 1:
            printf("探したい英単語を入力してください\n");
            scanf("%s",s);
            j=search_list(word,s);
            if(j!=0){
            printf("日本語訳は %s です\n",word[j-1].jp_word);
            }else{
                printf("単語が見つかりませんでした\n");
            }
            break;
        default:
            printf("指定されませんでした\nもう一度やり直してください\n");
            break;
    }
}
void syokika_struct(struct list word[TOTAL_LIST_LENGTH]){
    //structを初期化します
    int i;
    for(i=0;i<LENGTH_OF_WORD;i++)word[i].num=0;
    word[0].num=1;
    for(i=0;i<LENGTH_OF_WORD;i++)word[i].en_word[0]='\0';
    for(i=0;i<LENGTH_OF_WORD;i++)word[i].jp_word[0]='\0';
}

int main(){
    struct list word[TOTAL_LIST_LENGTH];
    char i;
    struct_read(word);
    printf("モードを入力してください\n1:単語追加モード\n2:練習モード(すべて)\n3:練習モード（過去に間違えたもののみ）\n4:日本語訳探し\n0:終了\n");
    scanf("%c",&i);
    switch(i){
        case '0':
            printf("終了します。\n");
            break;
        case '1':
            add_word(word);
            break;
        case '2':
            test_all(word);
            break;
        case '3':
            test_miss_only(word);
            break;
        case '4':
            search_word(word);
            break;
        default:
            printf("指定されませんでした。終了します\n");
            break;
    }
    struct_write(word);
}