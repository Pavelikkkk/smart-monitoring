import { HomeAudience } from "./home/HomeAudience";
import { HomeFeatures } from "./home/HomeFeatures";
import { HomeFooter } from "./home/HomeFooter";
import { HomeHero } from "./home/HomeHero";
import { HomeOverview } from "./home/HomeOverview";
import { HomeStats } from "./home/HomeStats";

export default function Home() {
  return (
    <div className="w-full">
      <HomeHero />
      <HomeStats />
      <HomeAudience />
      <HomeFeatures />
      <HomeOverview />
      <HomeFooter />
    </div>
  );
}
