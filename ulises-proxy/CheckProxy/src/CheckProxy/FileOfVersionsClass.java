package CheckProxy;

import java.util.ArrayList;
import java.util.List;

public class FileOfVersionsClass
{
	public String local_node;
	public String node_status;
	public String BKKSipProxyPBXVersion;
	public String PresenceServer;
	public String CentosRelease;
	public List<VersionProxy.FileAttr> components;
	
	FileOfVersionsClass()
	{
		node_status = "inactive";
		BKKSipProxyPBXVersion = "";
		PresenceServer = "";
		CentosRelease = "";
		components = new ArrayList<VersionProxy.FileAttr>();
	}	
}



