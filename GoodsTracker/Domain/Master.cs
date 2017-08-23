using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class Master : BaseCommunication
    {
        // Server
        public const int ADDRESS = 1;

        protected override void onReceiveAnswer(AnsCmd ans)
        {
            throw new NotImplementedException();
        }

        Master():base(ADDRESS)
        {

        }
    }
}
