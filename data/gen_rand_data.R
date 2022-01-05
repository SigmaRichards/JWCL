library(stringi)

overlap_prop = 0.1
d1_prop = 0.8
st_er1 = 0.05
num_people = 1000000


rand_names1 <- unique(stri_rand_strings(num_people/150, sample(3:9, num_people/150, replace=TRUE),"[A-M]"))
rand_names2 <- unique(stri_rand_strings(num_people/150, sample(3:9, num_people/150, replace=TRUE),"[M-Z]"))

by = as.character(sample(1900:2000,num_people,replace = TRUE))
bm = as.character(sample(1:30,num_people,replace = TRUE))
bd = as.character(sample(1:12,num_people,replace = TRUE))
fn = sample(rand_names1,num_people,replace = TRUE)
ln = sample(rand_names2,num_people,replace = TRUE)


people_overlap = sample(1:num_people,overlap_prop*num_people,replace = FALSE)
not_overlap = which(!1:num_people %in% people_overlap)
people_d1 = sample(not_overlap,d1_prop*length(not_overlap),replace = FALSE)
people_d2 = which(!1:num_people %in% c(people_d1,people_overlap))

df1 = data.frame(fn = fn[c(people_overlap,people_d1)],
                 ln = ln[c(people_overlap,people_d1)],
                 by = by[c(people_overlap,people_d1)],
                 bm = bm[c(people_overlap,people_d1)],
                 bd = bd[c(people_overlap,people_d1)],
                 stringsAsFactors = FALSE)
df2 = data.frame(fn = fn[c(people_overlap,people_d2)],
                 ln = ln[c(people_overlap,people_d2)],
                 by = by[c(people_overlap,people_d2)],
                 bm = bm[c(people_overlap,people_d2)],
                 bd = bd[c(people_overlap,people_d2)],
                 stringsAsFactors = FALSE)

sum(duplicated(df1))
sum(duplicated(df2))
sum(do.call(paste0, df1) %in% do.call(paste0, df2))

rand_se <- function(s){
  doer = ceiling(runif(length(s))*stri_length(s))
  substr(s,doer,doer)<-sample(LETTERS,length(s),replace = TRUE)
  return(s)
}
noisy_df <- function(df){
  for(c in 1:ncol(df)){
    er <- which(runif(nrow(df))<st_er1)
    df[er,c] <- rand_se(df[er,c])
  }
  return(df)
}
df1n <- noisy_df(df1)
df1n <- noisy_df(df1n)
df1n <- noisy_df(df1n)
df2n <- noisy_df(df2)
df2n <- noisy_df(df2n)
df2n <- noisy_df(df2n)

sum(do.call(paste0, df1n) %in% do.call(paste0, df2n))/(num_people*overlap_prop)

df1n$is_dupe = FALSE
df1n$is_dupe[1:length(people_overlap)] = TRUE
df2n$is_dupe = FALSE
df2n$is_dupe[1:length(people_overlap)] = TRUE

df1n <- df1n[sample(nrow(df1n)),]
df2n <- df1n[sample(nrow(df2n)),]

write.csv(df1n,file = "artificial_records1.csv",row.names = FALSE)
write.csv(df2n,file = "artificial_records2.csv",row.names = FALSE)