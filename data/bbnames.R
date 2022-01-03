overlap_prop = 0.1
d1_prop = 0.5
st_er1 = 0.1
st_er2 = 0.1
num_people = 100000


bbdf <- read.csv("baby-names-by-state.csv",stringsAsFactors = FALSE)
year_prop = unique(bbdf$year - min(bbdf$year))^2
year_prop = year_prop/sum(year_prop)


by = sample(unique(bbdf$year),num_people,prob = year_prop,replace = TRUE)
fn = sample(unique(bbdf$name),num_people,replace = TRUE)
ln = sample(unique(bbdf$name),num_people,replace = TRUE)


people_overlap = sample(1:num_people,overlap_prop*num_people,replace = FALSE)
not_overlap = which(!1:num_people %in% people_overlap)
people_d1 = sample(not_overlap,d1_prop*length(not_overlap),replace = FALSE)
people_d2 = which(!not_overlap %in% people_d1)

df1 = data.frame(fn = fn[c(people_overlap,people_d1)],
                 ln = ln[c(people_overlap,people_d1)],
                 by = by[c(people_overlap,people_d1)],
                 stringsAsFactors = FALSE)
df2 = data.frame(fn = fn[c(people_overlap,people_d2)],
                 ln = ln[c(people_overlap,people_d2)],
                 by = by[c(people_overlap,people_d2)],
                 stringsAsFactors = FALSE)

rand_se <- function(s){
  doer = which(runif(stri_length(df1$fn))<st_er2)
  for(a in doer){
    substr(s,a,a)<-sample(c(letters,LETTERS,0:9),1)
  }
  return(s)
}


er1 = which(runif(nrow(df1))<st_er1)
rand_se(df1$fn[2])
sapply(df1$fn[er1],rand_se)
