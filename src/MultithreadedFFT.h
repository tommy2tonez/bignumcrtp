
#ifndef __MULTITHREAD_FFT__
#define __MULTITHREAD_FFT__

#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/chrono/thread_clock.hpp>
#include <unordered_set>
#include <boost/functional/hash/hash.hpp>

#include "FFT.h"

namespace multithreading_fft{

    const uint8_t SPLIT_JOB = 0;
    const uint8_t MERGE_JOB = 1; 

    struct Job{

        size_t identifier; 
        size_t initial_length; 

        fft::ComplexNumber * data;
        fft::ComplexNumber * temp_space;

        size_t offset; 
        size_t n; 

        uint8_t job_type;

    };

    class JobDistributer{

        private:

            boost::mutex mtx;

            size_t counter;

            std::vector<Job> jobs;

            std::unordered_set<std::tuple<size_t, size_t, size_t, size_t>, boost::hash<std::tuple<size_t, size_t, size_t, size_t>>> mset;
            std::unordered_set<size_t> finished_jobs;

        public:

            bool get_job(Job& rs){
                
                boost::lock_guard<boost::mutex> guard(this->mtx);

                if (this->jobs.size() != 0){
                    
                    rs = this->jobs.back();
                    this->jobs.pop_back();
 
                    return true;

                }

                return false;

            }

            void push_job(Job data){

                boost::lock_guard<boost::mutex> guard(this->mtx);

                if (data.job_type == SPLIT_JOB){
                    
                    this->jobs.push_back(data);

                    return;

                }

                if (data.initial_length == data.n){
                    
                    this->finished_jobs.insert(data.identifier);

                    return;

                }

                size_t counterpart_offset = this->get_ctr_part(data.n, data.offset);
                
                auto key = std::make_tuple(data.identifier, data.initial_length, data.n, counterpart_offset);
                auto ptr = this->mset.find(key); 
                
                if (ptr == this->mset.end()){
                    
                    this->mset.insert(std::make_tuple(data.identifier, data.initial_length, data.n, data.offset));

                    return;

                }
                    
                this->jobs.push_back(this->get_unified_job(data, counterpart_offset));
                this->mset.erase(ptr);

            }

            size_t create_job(fft::ComplexNumber * data, fft::ComplexNumber * temp_space, size_t n){
                
                boost::lock_guard<boost::mutex> guard(this->mtx);

                Job job;

                job.data = data;
                job.temp_space = temp_space;
                job.initial_length = n;
                job.n = n;
                job.offset = 0;
                job.identifier = this->counter++;
                job.job_type = SPLIT_JOB;

                this->jobs.push_back(job);

                return job.identifier;

            }

            bool is_done(size_t id){

                boost::lock_guard<boost::mutex> guard(this->mtx);

                return this->finished_jobs.find(id) != this->finished_jobs.end(); 

            }

        private:

            size_t get_ctr_part(size_t n, size_t offset){
                
                size_t unified_length = n << 1;

                if (offset % unified_length == 0){

                    return offset + n; 

                }

                return offset - n;

            }

            Job get_unified_job(Job job, size_t counterpart_offset){
                
                std::swap(job.data, job.temp_space);

                if (counterpart_offset < job.offset){

                    job.data = job.data - (job.offset - counterpart_offset) * 2;
                    job.temp_space = job.temp_space - (job.offset - counterpart_offset) * 2;

                    job.offset = counterpart_offset;

                }

                job.n *= 2;

                return job;

            }

    };

    class JobDistributerSingleton{

        public:

            std::shared_ptr<JobDistributer> get(){
                
                boost::lock_guard<boost::mutex> guard(JobDistributerSingleton::mtx);

                if (job_distributer == nullptr){

                    job_distributer = std::make_shared<JobDistributer>();

                }

                return job_distributer;

            }

        private:

            inline static boost::mutex mtx;
            inline static std::shared_ptr<JobDistributer> job_distributer = nullptr;

    };

    class FixedLengthFFTMultiplierRetriever: private fft::TunedFFTPrecalculatedMultiplierRetriever, public virtual fft::FFTMultiplierRetrievable{

        private:

            void * malloc_space; 

        public:
            
            FixedLengthFFTMultiplierRetriever(){

                this->malloc_space = std::malloc(TunedFFTPrecalculatedMultiplierRetriever::get_instantiation_cost(MAX_N)); 
                fft::TunedFFTPrecalculatedMultiplierRetriever::prep(TunedFFTPrecalculatedMultiplierRetriever::MAX_N, malloc_space);
            
            }

            ~FixedLengthFFTMultiplierRetriever(){

                std::free(this->malloc_space);

            }

            fft::ComplexNumber get(size_t idx, size_t n){
                
                return fft::TunedFFTPrecalculatedMultiplierRetriever::get(idx, n);

            }

    };

    class FFTMultiplierSingleton{

        public:

            std::shared_ptr<FixedLengthFFTMultiplierRetriever> get(){

                boost::lock_guard<boost::mutex> guard(this->mtx);

                if (this->obj == nullptr){

                    this->obj = std::make_shared<FixedLengthFFTMultiplierRetriever>();

                }

                return this->obj;

            }

            void prep_archive(fft::StandardArchivedFFTPrecalculatedMultiplierRetriever& mul_retriever){
                
                boost::lock_guard<boost::mutex> guard(this->mtx);

                const size_t prep_length = 1 << 27;
                
                void * mem_alloc = std::malloc(mul_retriever.get_instantiation_cost(prep_length));
                mul_retriever.prep(prep_length, mem_alloc);

                std::free(mem_alloc);

            }

        private:

            inline static boost::mutex mtx;
            inline static std::shared_ptr<FixedLengthFFTMultiplierRetriever> obj = nullptr;

    };

    class JobResolver{

        public:

            void operator() (size_t sleep_time_in_microseconds, size_t break_time_in_microseconds){
                
                auto job_distributer = JobDistributerSingleton().get(); 
                auto multiplier = fft::StandardArchivedFFTPrecalculatedMultiplierRetriever();

                FFTMultiplierSingleton().prep_archive(multiplier);

                Job job;

                while (true){

                    if (job_distributer->get_job(job)){
                        
                        if (job.job_type == MERGE_JOB){

                            this->resolve_merge_job(job, multiplier);
                            job_distributer->push_job(job);

                        } else{

                            if (this->is_base_case(job)){

                                this->solve_base_case(job, multiplier);
                                job.job_type = MERGE_JOB; 

                                job_distributer->push_job(job);

                            } else{

                                auto rs = this->resolve_split_job(job);
                                job_distributer->push_job(rs.first);
                                job_distributer->push_job(rs.second);

                            }

                        }

                        continue;

                    }

                    boost::this_thread::sleep_for(boost::chrono::microseconds(sleep_time_in_microseconds));

                }

            }

        private:

            fft::StandardComplexNumberPluser plus_ops;
            fft::StandardComplexNumberMultiplier multiply_ops;

            void resolve_merge_job(Job& job, fft::StandardArchivedFFTPrecalculatedMultiplierRetriever& multiplier_retriever){

                fft::ComplexNumber * data = job.data;
                fft::ComplexNumber * temp_space = job.temp_space;

                size_t length = job.n; 
                size_t half_length = length >> 1;

                for (size_t i = 0; i < length; i += 2){

                    data[i] = this->plus_ops.plus(temp_space[i], this->multiply_ops.multiply(temp_space[i + length], multiplier_retriever.get(i >> 1, length))); 
                    data[i + length] = this->plus_ops.plus(temp_space[i], this->multiply_ops.multiply(temp_space[i + length], multiplier_retriever.get((i >> 1) + half_length, length))); 

                }
                
            }

            std::pair<Job, Job> resolve_split_job(Job& job){

                size_t half_length = job.n >> 1; 
                size_t i = 0;

                for (i = 0; i < job.n; i += 2){

                    job.temp_space[i] = job.data[i << 1]; 
                    job.temp_space[i + job.n] = job.data[(i + 1) << 1]; 

                }    

                Job first_half;
                Job second_half; 

                first_half.identifier = job.identifier;
                first_half.initial_length = job.initial_length;
                first_half.data = job.temp_space;
                first_half.n = half_length;
                first_half.temp_space = job.data;
                first_half.offset = job.offset;
                first_half.job_type = job.job_type;

                second_half.identifier = job.identifier;
                second_half.initial_length = job.initial_length;
                second_half.data = job.temp_space + job.n;
                second_half.n = half_length;
                second_half.temp_space = job.data + job.n;
                second_half.offset = job.offset + half_length; 
                second_half.job_type = job.job_type;

                return std::pair<Job, Job>(first_half, second_half);

            }

            bool is_base_case(Job& job){

                return job.n <= (1 << 15);

            }
            
            void solve_base_case(Job& job, fft::StandardArchivedFFTPrecalculatedMultiplierRetriever& multiplier_retriever){

                this->transform(job.data, job.n, job.temp_space, multiplier_retriever);

            }

            void transform(fft::ComplexNumber * data, size_t length, fft::ComplexNumber * temp_space, fft::StandardArchivedFFTPrecalculatedMultiplierRetriever& multiplier_retriever){

                if (length <= 1){

                    return;

                }

                size_t half_length = length >> 1; 
                size_t i = 0;

                for (i = 0; i < length; i += 2){

                    temp_space[i] = data[i << 1]; 
                    temp_space[i + length] = data[(i + 1) << 1]; 

                }

                this->transform(temp_space, half_length, data, multiplier_retriever);
                this->transform(temp_space + length, half_length, data + length, multiplier_retriever);

                for (i = 0; i < length; i += 2){

                    data[i] = this->plus_ops.plus(temp_space[i], this->multiply_ops.multiply(temp_space[i + length], multiplier_retriever.get(i >> 1, length))); 
                    data[i + length] = this->plus_ops.plus(temp_space[i], this->multiply_ops.multiply(temp_space[i + length], multiplier_retriever.get((i >> 1) + half_length, length))); 

                }

            }

    };

    class MultithreadFFT: public virtual fft::InPlaceTransformable{

        private:

            boost::mutex mtx;

            std::shared_ptr<fft::ComplexNumberDividable> divide_ops;
            std::shared_ptr<fft::Conjugatable> conjugator;
            memory::linear::TempStorageGeneratable * temp_storage;
            
        public:

            MultithreadFFT(size_t num_threads,
                           std::shared_ptr<fft::ComplexNumberDividable> divide_ops,
                           std::shared_ptr<fft::Conjugatable> conjugator,
                           memory::linear::TempStorageGeneratable * temp_storage){
                
                this->divide_ops = divide_ops;
                this->conjugator = conjugator;
                this->temp_storage = temp_storage;

                this->spawn_workers(num_threads);

            }

            ~MultithreadFFT(){

            }

            void transform(fft::ComplexNumber * data, size_t length){
                
                boost::lock_guard<boost::mutex> guard(this->mtx);

                this->transform_no_mtx(data, length);

            }

            void inverse_transform(fft::ComplexNumber * data, size_t length){

                boost::lock_guard<boost::mutex> guard(this->mtx);

                auto cjg_lambda = [ops = this->conjugator](fft::ComplexNumber e){return ops->conjugate(e);};
                auto dv_lambda = [ops = this->divide_ops, length](fft::ComplexNumber e){return ops->divide(e, length);}; 

                fft::linear_transform(data, length, cjg_lambda);
                this->transform_no_mtx(data, length);
                fft::linear_transform(data, length, cjg_lambda);
                fft::linear_transform(data, length, dv_lambda);

            }

        private:

            void transform_no_mtx(fft::ComplexNumber * data, size_t length){
                
                assert(length != 0);
                assert((length & (length - 1)) == 0);

                this->temp_storage->enter_scope();

                fft::ComplexNumber * temp_space = (fft::ComplexNumber *) this->temp_storage->get(length * sizeof(fft::ComplexNumber) * 2);
                
                for (size_t i = 0; i < length * 2; i += 2){

                    temp_space[i] = data[i >> 1];

                }

                size_t id = JobDistributerSingleton().get()->create_job(temp_space, temp_space + 1, length);
                this->wait(id);

                for (size_t i = 0; i < length * 2; i += 2){

                    data[i >> 1] = temp_space[i];

                }

                this->temp_storage->exit_scope();

            }

            void wait(size_t id){
                
                auto job_distributor = JobDistributerSingleton().get();
                size_t break_time_in_microseconds = 1000;

                while (!job_distributor->is_done(id)){
                    
                    boost::this_thread::sleep_for(boost::chrono::microseconds(break_time_in_microseconds));

                } 

            }

            void spawn_workers(uint64_t num_threads){
                
                uint64_t i = 0;

                const uint64_t DEFAULT_WAIT_TIME_IN_MICROSECONDS = 1000;
                const uint64_t DEFAULT_BREAK_TIME_IN_MICROSECONDS = 10;

                boost::thread spawned;

                for (i = 0; i < num_threads; ++i){
                    
                    spawned = boost::thread(JobResolver(), DEFAULT_WAIT_TIME_IN_MICROSECONDS, DEFAULT_BREAK_TIME_IN_MICROSECONDS);
                    spawned.detach();

                }

            }

    };

    class MultithreadFFTGenerator{

        public:

            std::shared_ptr<fft::InPlaceTransformable> get(size_t num_threads, memory::linear::TempStorageGeneratable * allocator){

                auto divide_ops = std::shared_ptr<fft::StandardComplexNumberDivider>(new fft::StandardComplexNumberDivider());
                auto conjugator = std::shared_ptr<fft::StandardConjugator>(new fft::StandardConjugator());

                return std::make_shared<MultithreadFFT>(num_threads, divide_ops, conjugator, allocator);

            }

    };

    class MultithreadLinearConvolutorGenerator{

        public:
            
            std::shared_ptr<fft::LinearConvolutable> get(size_t num_threads, memory::linear::TempStorageGeneratable * allocator){

                auto fft = MultithreadFFTGenerator().get(num_threads, allocator); 
                auto multiply_ops = std::shared_ptr<fft::StandardComplexNumberMultiplier>(new fft::StandardComplexNumberMultiplier);
                auto projector = std::shared_ptr<fft::LinearProjector>(new fft::LinearProjector);

                return std::shared_ptr<fft::LinearConvolutor>(new fft::LinearConvolutor(fft, multiply_ops, projector, allocator)); 

            }

    };

};

namespace polynomial{
    
    class MultithreadingUINTConvolutatorGenerator: public virtual UINTConvolutorGeneratable{

        private:

            memory::linear::TempStorageGeneratable * allocator;
            size_t n;

        public:
            
            MultithreadingUINTConvolutatorGenerator(){

                this->allocator = nullptr;
                n = 3;

            }

            MultithreadingUINTConvolutatorGenerator& set_temp_storage(memory::linear::TempStorageGeneratable * allocator){
                
                this->allocator = allocator;

                return *this;

            }

            MultithreadingUINTConvolutatorGenerator& set_threads(size_t n){
                
                this->n = n;

                return *this;

            }

            std::shared_ptr<FFTLengthComputable> get_fft_length_computer(){

                return std::make_shared<FFTLengthComputer>(); 

            }

            std::shared_ptr<UINTConvolutable> get(){

                assert(this->allocator != nullptr);

                auto linear_convolutor = multithreading_fft::MultithreadLinearConvolutorGenerator().get(this->n, this->allocator);
                auto caster = std::make_shared<LinearSizeTDoubleCaster>(); 
                auto fft_length_computer = std::make_shared<FFTLengthComputer>(); 

                return std::shared_ptr<UINTConvolutable>(new UINTConvolutor(linear_convolutor, caster, fft_length_computer)); 

            }

    };

}

namespace fft_multiplication::first_generation{
    
    class MultithreadMultiplicationOperatorGenerator: public virtual BoolVectorMultiplicationOperatorGeneratable{

        private:

            memory::linear::TempStorageGeneratable * allocator;
            size_t n;

        public:

            MultithreadMultiplicationOperatorGenerator(){

                this->allocator = nullptr;
                this->n = 3;

            }

            MultithreadMultiplicationOperatorGenerator& set_temp_storage(memory::linear::TempStorageGeneratable * allocator){

                this->allocator = allocator;

                return *this;

            }
            
            MultithreadMultiplicationOperatorGenerator& set_threads(size_t n){

                this->n = n;

                return *this;

            }

            std::shared_ptr<BoolVectorMultipliable> get(){

                assert(this->allocator != nullptr);

                auto fuser = StandardBoolVectorFuserGenerator().get();
                auto discretizer = StandardBoolVectorDiscretizerGenerator().get();
                auto uint_convolutor = polynomial::MultithreadingUINTConvolutatorGenerator().set_temp_storage(this->allocator).set_threads(this->n).get();
                //auto fft_length_computer = polynomial::StandardUINTConvolutorGenerator().get_fft_length_computer();
                auto split_length_optimizer = std::make_shared<StandardSplitLengthOptimizer>();

                return std::make_shared<BoolVectorMultiplicationOperator>(discretizer, fuser, uint_convolutor, split_length_optimizer, this->allocator);

            }

    };

}
#endif